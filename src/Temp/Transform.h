#pragma once

#include "Quaternion.h"
#include "Vector3.h"

/**
* Class allow to create transform matrix from a given position, orientation and scale
*/
template<class T> class nTransform {
public:
	nTransform();
	explicit nTransform(const Point3<T>&, const Quaternion<T>& q = Quaternion<T>(), const Vector3<T>& scale = Vector3<T>(1.0, 1.0, 1.0));

	void setPosition(const Point3<T>&);
	const Point3<T>& getPosition() const;

	void setOrientation(const Quaternion<T>&);
	const Quaternion<T>& getOrientation() const;

	void setScale(const Vector3<T>&);
	const Vector3<T>& getScale() const;
	bool hasScaling() const;

	const Matrix4<T>& getPositionMatrix() const;
	const Matrix4<T>& getOrientationMatrix() const;
	const Matrix4<T>& getScaleMatrix() const;
	const Matrix4<T>& getTransformMatrix() const;

	nTransform<T> operator*(const nTransform<T>&) const;
	const nTransform<T>& operator*=(const nTransform<T>&);
	bool operator==(const nTransform<T>&) const;

private:
	Point3<T> pPosition;
	Quaternion<T> qOrientation;
	Vector3<T> vScale;

	Matrix4<T> mPosition;
	Matrix4<T> mOrientation;
	Matrix4<T> mScale;
	Matrix4<T> mTransform;
};