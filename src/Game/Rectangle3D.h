#pragma once

#include "Line3D.h"

template<class T> class Rectangle3D {
public:
	explicit Rectangle3D(const std::array<Point3<T>, 4>&);

	Point3<T> intersectPoint(const Line3D<T>&, bool&, bool&) const;

private:
	Point3<T> planeIntersectionPoint(const Line3D<T>&, bool&) const;
	bool pointInsideSquare(const Point3<T>&) const;

	std::array<Point3<T>, 4> orientedPoints;
	Vector3<T> normal;
};