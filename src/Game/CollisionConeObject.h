#pragma once

#include "CollisionConvexObject3D.h"

class CollisionConeObject final : public CollisionConvexObject3D {
public:
	CollisionConeObject(float, float, float, typename ConeShape<float>::ConeOrientation, const Point3<float>&, const Quaternion<float>&);

	float getRadius() const;
	float getHeight() const;
	typename ConeShape<float>::ConeOrientation getConeOrientation() const;
	const Point3<float>& getCenterOfMass() const;
	const Quaternion<float>& getOrientation() const;
	const Vector3<float>& getAxis(unsigned int) const;

	CollisionConvexObject3D::ObjectType getObjectType() const override;
	Point3<float> getSupportPoint(const Vector3<float>&, bool) const override;

	nCone<float> retrieveCone() const;
	std::string toString() const override;

private:
	const nCone<float> coneObject; //object without margin
};