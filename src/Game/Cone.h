#pragma once

#include "ConvexObject3D.h"
#include "ConeShape.h"
#include "Point3.h"
#include "Vector3.h"
#include "Quaternion.h"

template<class T> class nCone final : public ConvexObject3D<T> {
public:
	nCone();
	nCone(T, T, typename ConeShape<T>::ConeOrientation, const Point3<T>&, const Quaternion<T>&);

	T getRadius() const;
	T getHeight() const;
	typename ConeShape<T>::ConeOrientation getConeOrientation() const;
	const Point3<T>& getCenterOfMass() const;
	const Quaternion<T>& getOrientation() const;
	const Vector3<T>& getAxis(unsigned int) const;

	Point3<T> getCenter() const;
	Point3<T> getSupportPoint(const Vector3<T>&) const override;

private:
	ConeShape<T> coneShape;
	Point3<T> centerOfMass;
	Quaternion<T> orientation;

	std::array<Vector3<T>, 3> axis; //3 vectors of normalized axis
	T baseSideCosAngle;
};

template<class T> std::ostream& operator <<(std::ostream&, const nCone<T>&);