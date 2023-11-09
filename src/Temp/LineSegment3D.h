#pragma once

#include "ConvexObject3D.h"
#include "Line3D.h"
#include "Vector3.h"
#include "Point3.h"

template<class T> class LineSegment3D : public ConvexObject3D<T> {
public:
	LineSegment3D();
	LineSegment3D(const Point3<T>&, const Point3<T>&);

	const Point3<T>& getA() const;
	const Point3<T>& getB() const;

	Point3<T> getSupportPoint(const Vector3<T>&) const;
	Point3<T> closestPoint(const Point3<T>&) const;
	Point3<T> closestPoint(const Point3<T>&, std::array<T, 2>&) const;
	T squareDistance(const Point3<T>&) const;

	Vector3<T> toVector() const;
	Line3D<T> toLine() const;

private:
	Point3<T> a;
	Point3<T> b;
};

template<class T> std::ostream& operator <<(std::ostream&, const LineSegment3D<T>&);