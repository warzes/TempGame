#pragma once

#include "Common.h"

class nCamera : public Observable {
public:
	nCamera(float, float, float);
	~nCamera() override = default;

	enum NotificationType {
		PROJECTION_UPDATE,
		POSITION_UPDATED,
	};

	void initialize(unsigned int, unsigned int);
	void onResize(unsigned int, unsigned int);
	unsigned int getSceneWidth() const;
	unsigned int getSceneHeight() const;

	void useMouseToMoveCamera(bool);
	bool isUseMouseToMoveCamera() const;
	void setMouseSensitivityPercentage(float);
	void setInvertYAxis(bool);
	void setDistance(float);
	bool isFirstPersonCamera() const;
	void setMaxRotationX(float);

	const Matrix4<float>& getViewMatrix() const;
	const Matrix4<float>& getProjectionMatrix() const;

	//pre-computed values
	const Quaternion<float>& getOrientation() const;
	const Matrix4<float>& getProjectionViewMatrix() const;
	const Matrix4<float>& getProjectionViewInverseMatrix() const;
	const Matrix4<float>& getProjectionInverseMatrix() const;

	const Point3<float>& getPosition() const;
	const Vector3<float>& getView() const;
	const Vector3<float>& getUp() const;

	float getHorizontalFovAngle() const;
	void updateHorizontalFovAngle(float);
	float computeFov() const;
	float getNearPlane() const;
	float getFarPlane() const;
	const nFrustum<float>& getFrustum() const;

	void moveTo(const Point3<float>&);
	void moveOnLocalXAxis(float);
	void moveOnLocalZAxis(float);
	void rotate(const Quaternion<float>&);
	void updateViewUp(const Vector3<float>&, const Vector3<float>&);

	virtual bool onKeyPress();
	virtual bool onKeyRelease();
	virtual bool onMouseMove(double, double);

	virtual void moveForward(float);
	virtual void moveBackward(float);
	virtual void moveLeft(float);
	virtual void moveRight(float);

private:
	void initializeOrUpdate(unsigned int, unsigned int);
	float computeVerticalFovAngle() const;
	float computeFov(float) const;
	void updateComponents();

	const float MOUSE_SENSITIVITY_FACTOR;
	Matrix4<float> mView;
	Matrix4<float> mProjection;

	//pre-computed values
	Quaternion<float> orientation;
	Matrix4<float> mProjectionView;
	Matrix4<float> mProjectionViewInverse;
	Matrix4<float> mProjectionInverse;

	Point3<float> position;
	Vector3<float> view;
	Vector3<float> up;

	float horizontalFovAngle;
	float nearPlane;
	float farPlane;
	nFrustum<float> baseFrustum; //base frustum (without any matrix transformation)
	nFrustum<float> frustum;

	float maxRotationX;
	float distance; //distance between the camera and the rotation point (0 : first person camera | >0 : third person camera)
	bool useMouse; //true if the cursor is used to move the camera
	float mouseSensitivityPercentage;
	bool invertYAxis;
	unsigned int sceneWidth;
	unsigned int sceneHeight;
};