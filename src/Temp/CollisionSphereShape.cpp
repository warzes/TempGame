#include "stdafx.h"
#include "CollisionSphereShape.h"
#include "CollisionSphereObject.h"

/**
* @param innerMargin Collision inner margin (must be equals to sphere radius)
*/
CollisionSphereShape::CollisionSphereShape(float innerMargin) :
	CollisionShape3D(innerMargin),
	sphereShape(SphereShape<float>(innerMargin)) {

}

CollisionShape3D::ShapeType CollisionSphereShape::getShapeType() const {
	return CollisionShape3D::SPHERE_SHAPE;
}

const ConvexShape3D<float>& CollisionSphereShape::getSingleShape() const {
	return sphereShape;
}

float CollisionSphereShape::getRadius() const {
	return sphereShape.getRadius();
}

std::unique_ptr<CollisionShape3D> CollisionSphereShape::scale(const Vector3<float>& scale) const {
	if (!MathFunction::isEqual(scale.X, scale.Y, 0.01f) || !MathFunction::isEqual(scale.Y, scale.Z, 0.01f)) {
		LogWarning("Sphere cannot be correctly scaled with " + StringUtil::toString(scale) + ". Consider to use another shape.");
	}

	float radiusScale = std::min(scale.X, std::min(scale.Y, scale.Z));
	float newRadius = std::max(0.001f, sphereShape.getRadius() * radiusScale);
	return std::make_unique<CollisionSphereShape>(newRadius);
}

AABBox<float> CollisionSphereShape::toAABBox(const PhysicsTransform& physicsTransform) const {
	const Point3<float>& position = physicsTransform.getPosition();

	return AABBox<float>(position - sphereShape.getRadius(), position + sphereShape.getRadius());
}

std::unique_ptr<CollisionConvexObject3D, ObjectDeleter> CollisionSphereShape::toConvexObject(const PhysicsTransform& physicsTransform) const {
	const Point3<float>& position = physicsTransform.getPosition();

	void* memPtr = getObjectsPool().allocate(sizeof(CollisionSphereObject));
	auto* collisionObjectPtr = new (memPtr) CollisionSphereObject(getInnerMargin(), position);
	return std::unique_ptr<CollisionSphereObject, ObjectDeleter>(collisionObjectPtr);
}

Vector3<float> CollisionSphereShape::computeLocalInertia(float mass) const {
	float localInertia = (2.0f / 5.0f) * mass * sphereShape.getRadius() * sphereShape.getRadius();
	return Vector3<float>(localInertia, localInertia, localInertia);
}

float CollisionSphereShape::getMaxDistanceToCenter() const {
	return sphereShape.getRadius();
}

float CollisionSphereShape::getMinDistanceToCenter() const {
	return sphereShape.getRadius();
}

std::unique_ptr<CollisionShape3D> CollisionSphereShape::clone() const {
	return std::make_unique<CollisionSphereShape>(sphereShape.getRadius());
}