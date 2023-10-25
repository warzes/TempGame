#pragma once

#include "ConvexObject3D.h"
#include "CapsuleShape.h"
#include "Point3.h"
#include "Vector3.h"
#include "Quaternion.h"

template<class T> class nCapsule final : public ConvexObject3D<T> {
public:
	nCapsule();
	nCapsule(T, T, typename CapsuleShape<T>::CapsuleOrientation, const Point3<T>&, const Quaternion<T>&);

	T getRadius() const;
	T getCylinderHeight() const;
	typename CapsuleShape<T>::CapsuleOrientation getCapsuleOrientation() const;
	const Point3<T>& getCenterOfMass() const;
	const Quaternion<T>& getOrientation() const;
	const Vector3<T>& getAxis(unsigned int) const;

	Point3<T> getSupportPoint(const Vector3<T>&) const override;

private:
	CapsuleShape<T> capsuleShape;
	Point3<T> centerOfMass;
	Quaternion<T> orientation;

	std::array<Vector3<T>, 3> axis; //3 vectors of normalized axis
};

template<class T> std::ostream& operator <<(std::ostream&, const nCapsule<T>&);