#pragma once

#include "Line3D.h"
#include "Point4.h"

template<class T> class nPlane
{
public:
	nPlane();
	nPlane(const Vector3<T>&, T);
	nPlane(const Point3<T>&, const Point3<T>&, const Point3<T>&);
	nPlane(const Vector3<T>&, const Point3<T>&);

	void buildFrom3Points(const Point3<T>&, const Point3<T>&, const Point3<T>&);
	void buildFromNormalAndPoint(const Vector3<T>&, const Point3<T>&);
	void reverse();

	const Vector3<T>& getNormal() const;
	void setNormal(const Vector3<T>&);
	T getDistanceToOrigin() const;
	void setDistanceToOrigin(T);

	T distance(const Point3<T>&) const;
	T distance(const Point4<T>&) const;
	T verticalDistance(const Point3<T>&) const;
	T horizontalDistance(const Point3<T>&) const;
	Point3<T> orthogonalProjection(const Point3<T>&) const;

	Point3<T> intersectPoint(const Line3D<T>&, bool&) const;

private:
	Vector3<T> normal; //plane normal normalized
	T d; //Distance to the origin. Positive if dot product between a vector from plane to origin and the normal is positive
};

template<class T> std::ostream& operator <<(std::ostream&, const nPlane<T>&);