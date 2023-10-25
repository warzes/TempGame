#include "stdafx.h"
#include "Transform.h"
#include "MathFunction.h"

template<class T> nTransform<T>::nTransform() :
	vScale(Vector3<T>(1.0, 1.0, 1.0)) {
	pPosition.setNull();
	qOrientation.setIdentity();
	//not need to compute transform matrix: it's the identity matrix
}

template<class T> nTransform<T>::nTransform(const Point3<T>& position, const Quaternion<T>& orientation, const Vector3<T>& scale) :
	pPosition(position),
	qOrientation(orientation),
	vScale(scale),
	mOrientation(orientation.toMatrix4()) {
	mPosition = Matrix4<T>::buildTranslation(pPosition.X, pPosition.Y, pPosition.Z);

	mScale = Matrix4<T>::buildScale(vScale.X, vScale.Y, vScale.Z);
	mTransform = mPosition * mOrientation * mScale;
}

template<class T> void nTransform<T>::setPosition(const Point3<T>& position) {
	pPosition = position;

	mPosition = Matrix4<T>::buildTranslation(pPosition.X, pPosition.Y, pPosition.Z);
	mTransform = mPosition * mOrientation * mScale;
}

template<class T> const Point3<T>& nTransform<T>::getPosition() const {
	return pPosition;
}

template<class T> void nTransform<T>::setOrientation(const Quaternion<T>& orientation) {
	qOrientation = orientation;

	mOrientation = qOrientation.toMatrix4();
	mTransform = mPosition * mOrientation * mScale;
}

template<class T> const Quaternion<T>& nTransform<T>::getOrientation() const {
	return qOrientation;
}

template<class T> void nTransform<T>::setScale(const Vector3<T>& scale) {
	vScale = scale;

	mScale = Matrix4<T>::buildScale(vScale.X, vScale.Y, vScale.Z);
	mTransform = mPosition * mOrientation * mScale;
}

template<class T> const Vector3<T>& nTransform<T>::getScale() const {
	return vScale;
}

template<class T> bool nTransform<T>::hasScaling() const {
	return !MathFunction::isOne((T)vScale.X, (T)0.001) || !MathFunction::isOne((T)vScale.Y, (T)0.001) || !MathFunction::isOne((T)vScale.Z, (T)0.001);
}

template<class T> const Matrix4<T>& nTransform<T>::getPositionMatrix() const {
	return mPosition;
}

template<class T> const Matrix4<T>& nTransform<T>::getOrientationMatrix() const {
	return mOrientation;
}

template<class T> const Matrix4<T>& nTransform<T>::getScaleMatrix() const {
	return mScale;
}

template<class T> const Matrix4<T>& nTransform<T>::getTransformMatrix() const {
	return mTransform;
}

template<class T> nTransform<T> nTransform<T>::operator *(const nTransform<T>& transform) const {
	return nTransform<T>(pPosition + qOrientation.rotatePoint(transform.getPosition()),
		qOrientation * transform.getOrientation(),
		vScale * transform.getScale());
}

template<class T> const nTransform<T>& nTransform<T>::operator *=(const nTransform<T>& transform) {
	*this = *this * transform;
	return *this;
}

template<class T> bool nTransform<T>::operator ==(const nTransform<T>& t) const {
	return pPosition == t.getPosition() && qOrientation == t.getOrientation() && vScale == t.getScale();
}

//explicit template
template class nTransform<float>;
template class nTransform<double>;