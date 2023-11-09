#pragma once

#include "ConvexObject3D.h"
#include "CylinderShape.h"
#include "Point3.h"
#include "Vector3.h"
#include "Quaternion.h"

template<class T> class nCylinder final : public ConvexObject3D<T> 
{
public:
	nCylinder();
	nCylinder(T, T, typename CylinderShape<T>::CylinderOrientation, const Point3<T>&, const Quaternion<T>&);

	T getRadius() const;
	T getHeight() const;
	typename CylinderShape<T>::CylinderOrientation getCylinderOrientation() const;
	const Point3<T>& getCenterOfMass() const;
	const Quaternion<T>& getOrientation() const;
	const Vector3<T>& getAxis(unsigned int) const;

	Point3<T> getSupportPoint(const Vector3<T>&) const override;

private:
	CylinderShape<T> cylinderShape;
	Point3<T> centerOfMass;
	Quaternion<T> orientation;

	std::array<Vector3<T>, 3> axis; //3 vectors of normalized axis
};

template<class T> std::ostream& operator <<(std::ostream&, const nCylinder<T>&);