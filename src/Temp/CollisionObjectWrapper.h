#pragma once

#include "CollisionShape3D.h"
#include "PhysicsTransform.h"

/**
* Wrapper for object used in collision algorithms
*/
class CollisionObjectWrapper {
public:
	CollisionObjectWrapper(const CollisionShape3D&, const PhysicsTransform&);

	const CollisionShape3D& getShape() const;
	const PhysicsTransform& getShapeWorldTransform() const;

private:
	const CollisionShape3D& shape;
	PhysicsTransform shapeWorldTransform;
};