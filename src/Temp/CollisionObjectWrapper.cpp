#include "stdafx.h"
#include "CollisionObjectWrapper.h"

CollisionObjectWrapper::CollisionObjectWrapper(const CollisionShape3D& shape, const PhysicsTransform& shapeWorldTransform) :
	shape(shape),
	shapeWorldTransform(shapeWorldTransform) {

}

const CollisionShape3D& CollisionObjectWrapper::getShape() const {
	return shape;
}

const PhysicsTransform& CollisionObjectWrapper::getShapeWorldTransform() const {
	return shapeWorldTransform;
}