#include "stdafx.h"
#include "Camera.h"

nCamera::nCamera(float horizontalFovAngle, float nearPlane, float farPlane) :
	MOUSE_SENSITIVITY_FACTOR(0.003/*ConfigService::instance().getFloatValue("camera.mouseSensitivityFactor")*/),
	mView(Matrix4<float>()),
	mProjection(Matrix4<float>()),
	position(Point3<float>(0.0f, 0.0f, 0.0f)),
	view(Vector3<float>(0.0f, 0.0f, -1.0f)),
	up(Vector3<float>(0.0f, 1.0f, 0.0f)),
	horizontalFovAngle(horizontalFovAngle),
	nearPlane(nearPlane),
	farPlane(farPlane),
	maxRotationX(0.995f),
	distance(0.0f),
	useMouse(false),
	mouseSensitivityPercentage(1.0f),
	invertYAxis(false),
	sceneWidth(0),
	sceneHeight(0) {
	float verticalFovAngle = computeVerticalFovAngle();
	baseFrustum = nFrustum<float>(verticalFovAngle, 1.0f, nearPlane, farPlane);
	frustum = nFrustum<float>(verticalFovAngle, 1.0f, nearPlane, farPlane);
}

void nCamera::initialize(unsigned int sceneWidth, unsigned int sceneHeight) {
	initializeOrUpdate(sceneWidth, sceneHeight);
}

void nCamera::onResize(unsigned int sceneWidth, unsigned int sceneHeight) {
	initializeOrUpdate(sceneWidth, sceneHeight);
}

unsigned int nCamera::getSceneWidth() const {
	return sceneWidth;
}

unsigned int nCamera::getSceneHeight() const {
	return sceneHeight;
}

void nCamera::initializeOrUpdate(unsigned int sceneWidth, unsigned int sceneHeight) {
	this->sceneWidth = sceneWidth;
	this->sceneHeight = sceneHeight;

	//projection matrix
	float verticalFovAngle = computeVerticalFovAngle();
	float fov = computeFov(verticalFovAngle);
	float ratio = (float)sceneWidth / (float)sceneHeight;
	mProjection.setValues(
		fov / ratio, 0.0f, 0.0f, 0.0f,
		0.0f, -fov, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f * ((farPlane + nearPlane) / (nearPlane - farPlane)) - 0.5f, (farPlane * nearPlane) / (nearPlane - farPlane),
		0.0f, 0.0f, -1.0f, 0.0f);

	baseFrustum.buildFrustum(verticalFovAngle, ratio, nearPlane, farPlane);

	updateComponents();
	notifyObservers(this, nCamera::PROJECTION_UPDATE);
}

float nCamera::computeVerticalFovAngle() const {
	if (sceneHeight != 0 && sceneWidth != 0) {
		float horizontalFovRadian = AngleConverter<float>::toRadian(horizontalFovAngle);
		float verticalFovRadian = 2.0f * std::atan(std::tan(horizontalFovRadian / 2.0f) * (float)sceneHeight / (float)sceneWidth);
		return AngleConverter<float>::toDegree(verticalFovRadian);
	}
	return 1.0f;
}

void nCamera::useMouseToMoveCamera(bool use) {
	useMouse = use;
}

bool nCamera::isUseMouseToMoveCamera() const {
	return useMouse;
}

void nCamera::setMouseSensitivityPercentage(float mouseSensitivityPercentage) {
	this->mouseSensitivityPercentage = mouseSensitivityPercentage;
}

void nCamera::setInvertYAxis(bool invertYAxis) {
	this->invertYAxis = invertYAxis;
}

/**
* @param distance Distance between the camera and the rotation point (0 : first person camera | >0 : third person camera)
*/
void nCamera::setDistance(float distance) {
	this->distance = distance;
}

bool nCamera::isFirstPersonCamera() const {
	return MathFunction::isZero(distance);
}

void nCamera::setMaxRotationX(float maxRotationX) {
	this->maxRotationX = maxRotationX;
}

const Matrix4<float>& nCamera::getViewMatrix() const {
	return mView;
}

const Matrix4<float>& nCamera::getProjectionMatrix() const {
	return mProjection;
}

const Matrix4<float>& nCamera::getProjectionViewMatrix() const {
	return mProjectionView;
}

const Matrix4<float>& nCamera::getProjectionViewInverseMatrix() const {
	return mProjectionViewInverse;
}

const Matrix4<float>& nCamera::getProjectionInverseMatrix() const {
	return mProjectionInverse;
}

const Quaternion<float>& nCamera::getOrientation() const {
	return orientation;
}

const Point3<float>& nCamera::getPosition() const {
	return position;
}

const Vector3<float>& nCamera::getView() const {
	return view;
}

const Vector3<float>& nCamera::getUp() const {
	return up;
}

float nCamera::getHorizontalFovAngle() const {
	return horizontalFovAngle;
}

void nCamera::updateHorizontalFovAngle(float horizontalFovAngle) {
	if (!MathFunction::isEqual(horizontalFovAngle, this->horizontalFovAngle, 0.001f)) {
		this->horizontalFovAngle = horizontalFovAngle;
		initializeOrUpdate(sceneWidth, sceneHeight);
	}
}

float nCamera::computeFov() const {
	float verticalFovAngle = computeVerticalFovAngle();
	return computeFov(verticalFovAngle);
}

float nCamera::computeFov(float verticalFovAngle) const {
	return 1.0f / std::tan((verticalFovAngle * MathValue::PI_FLOAT) / 360.0f);
}

float nCamera::getNearPlane() const {
	return nearPlane;
}

float nCamera::getFarPlane() const {
	return farPlane;
}

const nFrustum<float>& nCamera::getFrustum() const {
	return frustum;
}

void nCamera::moveTo(const Point3<float>& position) {
	if (this->position != position) {
		this->position = position;

		updateComponents();
		notifyObservers(this, nCamera::POSITION_UPDATED);
	}
}

void nCamera::moveOnLocalXAxis(float distance) {
	Vector3<float> localXAxis = up.crossProduct(view).normalize();
	position = position.translate(localXAxis * distance);

	updateComponents();
	notifyObservers(this, nCamera::POSITION_UPDATED);
}

void nCamera::moveOnLocalZAxis(float distance) {
	Vector3<float> localZAxis = view;
	position = position.translate(localZAxis * distance);

	updateComponents();
	notifyObservers(this, nCamera::POSITION_UPDATED);
}

void nCamera::rotate(const Quaternion<float>& rotationDelta) {
	Point3<float> pivot = position;
	if (!isFirstPersonCamera()) {
		pivot = position.translate(view * distance);
	}

	//moves view point
	Point3<float> viewPoint = pivot.translate(rotationDelta.rotateVector(view));
	view = position.vector(viewPoint).normalize();

	//moves up vector
	up = rotationDelta.rotateVector(up);
	up = up.normalize(); //normalize is required because due to float imprecision, the vector becomes less and less normalized

	//moves position point
	if (!isFirstPersonCamera()) {
		Vector3<float> axis = pivot.vector(position);
		position = pivot.translate(rotationDelta.rotateVector(axis));

		updateComponents();
		notifyObservers(this, nCamera::POSITION_UPDATED);
	}
	else {
		updateComponents();
	}
}

void nCamera::updateViewUp(const Vector3<float>& view, const Vector3<float>& up) {
	this->view = view;
	this->up = up;
	updateComponents();
}

bool nCamera::onKeyPress() {
	//do nothing
	return true;
}

bool nCamera::onKeyRelease() {
	//do nothing
	return true;
}

bool nCamera::onMouseMove(double deltaMouseX, double deltaMouseY) {
	if (useMouse) {
		if (deltaMouseX == 0.0 && deltaMouseY == 0.0) {
			return false;
		}

		Vector2<float> mouseDirection;
		mouseDirection.X = (float)(-deltaMouseX * (double)MOUSE_SENSITIVITY_FACTOR * (double)mouseSensitivityPercentage);
		mouseDirection.Y = (float)(-deltaMouseY * (double)MOUSE_SENSITIVITY_FACTOR * (double)mouseSensitivityPercentage);
		if (invertYAxis) {
			mouseDirection.Y = -mouseDirection.Y;
		}

		//do not rotate up/down more than "maxRotationX" percent
		float currentRotationX = view.Y + mouseDirection.Y;
		if (currentRotationX > 0.0f && currentRotationX > maxRotationX) {
			mouseDirection.Y -= (currentRotationX - maxRotationX);
		}
		else if (currentRotationX < 0.0f && currentRotationX < -maxRotationX) {
			mouseDirection.Y -= (currentRotationX + maxRotationX);
		}

		//rotate around the Y and X axis
		rotate(Quaternion<float>::fromAxisAngle(up.crossProduct(view), -mouseDirection.Y));
		rotate(Quaternion<float>::rotationY(mouseDirection.X));

		updateComponents();

		return false;
	}
	return true;
}

void nCamera::updateComponents() {
	const Vector3<float>& viewUp = view.crossProduct(up).normalize();
	mView.setValues(
		viewUp[0], viewUp[1], viewUp[2], viewUp[0] * -position.X + viewUp[1] * -position.Y + viewUp[2] * -position.Z,
		up[0], up[1], up[2], up[0] * -position.X + up[1] * -position.Y + up[2] * -position.Z,
		-view[0], -view[1], -view[2], -view[0] * -position.X + -view[1] * -position.Y + -view[2] * -position.Z,
		0.0f, 0.0f, 0.0f, 1.0f);
	frustum = baseFrustum * mView.inverse();

	//pre-computed values
	orientation = Quaternion<float>::fromRotationMatrix(mView.toMatrix3()).conjugate();
	mProjectionView = mProjection * mView;
	mProjectionViewInverse = mProjectionView.inverse();
	mProjectionInverse = mProjection.inverse();
}

void nCamera::moveForward(float) {
	//do nothing (can be overridden)
}

void nCamera::moveBackward(float) {
	//do nothing (can be overridden)
}

void nCamera::moveLeft(float) {
	//do nothing (can be overridden)
}

void nCamera::moveRight(float) {
	//do nothing (can be overridden)
}