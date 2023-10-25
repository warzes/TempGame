#pragma once

#include "CollisionShape3D.h"
#include "CollisionConvexObject3D.h"
#include "PhysicsTransform.h"

struct LocalizedCollisionShape {
	std::size_t shapeIndex;

	std::unique_ptr<const CollisionShape3D> shape;
	PhysicsTransform transform;
};

class CollisionCompoundShape final : public CollisionShape3D {
public:
	explicit CollisionCompoundShape(std::vector<std::shared_ptr<const LocalizedCollisionShape>>);

	CollisionShape3D::ShapeType getShapeType() const override;
	const ConvexShape3D<float>& getSingleShape() const override;
	const std::vector<std::shared_ptr<const LocalizedCollisionShape>>& getLocalizedShapes() const;

	std::unique_ptr<CollisionShape3D> scale(const Vector3<float>&) const override;

	AABBox<float> toAABBox(const PhysicsTransform&) const override;
	std::unique_ptr<CollisionConvexObject3D, ObjectDeleter> toConvexObject(const PhysicsTransform&) const override;

	Vector3<float> computeLocalInertia(float) const override;
	float getMaxDistanceToCenter() const override;
	float getMinDistanceToCenter() const override;

	std::unique_ptr<CollisionShape3D> clone() const override;

private:
	void initializeDistances();

	const std::vector<std::shared_ptr<const LocalizedCollisionShape>> localizedShapes;

	float maxDistanceToCenter;
	float minDistanceToCenter;
};