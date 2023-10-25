#pragma once

#include "CollisionConvexObject3D.h"

class CollisionSphereObject final : public CollisionConvexObject3D {
public:
	CollisionSphereObject(float, const Point3<float>&);

	float getRadius() const;
	const Point3<float>& getCenterOfMass() const;

	CollisionConvexObject3D::ObjectType getObjectType() const override;
	Point3<float> getSupportPoint(const Vector3<float>&, bool) const override;

	nSphere<float> retrieveSphere() const;
	std::string toString() const override;

private:
	const nSphere<float> sphereObject; //object without margin
};