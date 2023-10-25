#pragma once

#include "Point3.h"

template<class T> class nRay {
public:
	nRay(const Point3<T>&, const Vector3<T>&, T);
	nRay(const Point3<T>&, const Point3<T>&);

	const Point3<T>& getOrigin() const;
	Point3<T> computeTo() const;

	const Vector3<T>& getDirection() const;
	T getLength() const;

	const Vector3<T>& getInverseDirection() const;
	unsigned int getDirectionSign(unsigned int) const;

private:
	void initializeAdditionalData();

	Point3<T> origin;
	Vector3<T> direction; //normalized
	T length;

	//additional data for performance (ray tracing, collision...)
	Vector3<T> inverseDirection;
	std::array<unsigned int, 3> directionSigns;
};