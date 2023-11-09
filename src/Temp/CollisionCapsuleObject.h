#pragma once

#include "CollisionConvexObject3D.h"

class CollisionCapsuleObject final : public CollisionConvexObject3D {
public:
	CollisionCapsuleObject(float, float, float, typename CapsuleShape<float>::CapsuleOrientation, const Point3<float>&, const Quaternion<float>&);

	float getRadius() const;
	float getCylinderHeight() const;
	typename CapsuleShape<float>::CapsuleOrientation getCapsuleOrientation() const;
	const Point3<float>& getCenterOfMass() const;
	const Quaternion<float>& getOrientation() const;
	const Vector3<float>& getAxis(unsigned int) const;

	CollisionConvexObject3D::ObjectType getObjectType() const override;
	Point3<float> getSupportPoint(const Vector3<float>&, bool) const override;

	nCapsule<float> retrieveCapsule() const;
	std::string toString() const override;

private:
	const nCapsule<float> capsuleObject; //object without margin
};