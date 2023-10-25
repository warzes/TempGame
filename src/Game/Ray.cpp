#include "stdafx.h"
#include "Ray.h"

template<class T> nRay<T>::nRay(const Point3<T>& origin, const Vector3<T>& direction, T length) :
	origin(origin),
	direction(direction.normalize()),
	length(length),
	directionSigns() {
	initializeAdditionalData();
}

template<class T> nRay<T>::nRay(const Point3<T>& from, const Point3<T>& to) :
	origin(from),
	directionSigns() {
	Vector3<T> direction = from.vector(to);
	this->length = direction.length();
	this->direction = direction / length;

	initializeAdditionalData();
}

template<class T> void nRay<T>::initializeAdditionalData() {
	inverseDirection = Vector3<T>(1.0, 1.0, 1.0) / this->direction;

	directionSigns[0] = this->direction.X < 0.0 ? 1 : 0;
	directionSigns[1] = this->direction.Y < 0.0 ? 1 : 0;
	directionSigns[2] = this->direction.Z < 0.0 ? 1 : 0;
}

template<class T> const Point3<T>& nRay<T>::getOrigin() const {
	return origin;
}

template<class T> Point3<T> nRay<T>::computeTo() const {
	return origin.translate(direction * length);
}

template<class T> const Vector3<T>& nRay<T>::getDirection() const {
	return direction;
}

template<class T> T nRay<T>::getLength() const {
	return length;
}

template<class T> const Vector3<T>& nRay<T>::getInverseDirection() const {
	return inverseDirection;
}

/**
* @return Zero for positive direction otherwise one for negative direction
*/
template<class T> unsigned int nRay<T>::getDirectionSign(unsigned int index) const {
	return directionSigns[index];
}

//explicit template
template class nRay<float>;
template class nRay<double>;