#pragma once

#include "ConvexObject3D.h"
#include "SphereShape.h"
#include "Point3.h"

template<class T> class nSphere final : public ConvexObject3D<T> {
public:
	nSphere();
	nSphere(T, const Point3<T>&);

	T getRadius() const;
	const Point3<T>& getCenterOfMass() const;

	Point3<T> getSupportPoint(const Vector3<T>&) const override;

	bool collideWithPoint(const Point3<T>&) const override;
	bool collideWithSphere(const nSphere<T>&) const override;
	bool collideWithAABBox(const AABBox<T>&) const override;
	Point3<T> intersectPoint(const Line3D<T>&, bool&) const override;

private:
	SphereShape<T> sphereShape;
	Point3<T> centerOfMass;
};

template<class T> std::ostream& operator <<(std::ostream&, const nSphere<T>&);