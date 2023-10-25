#pragma once

#include "CollisionShape3D.h"
#include "CollisionConvexObject3D.h"
#include "CollisionTriangleObject.h"
#include "PhysicsTransform.h"
#include "FixedSizePool.h"

class CollisionTriangleShape final : public CollisionShape3D {
public:
	explicit CollisionTriangleShape(const std::array<Point3<float>, 3>&);
	explicit CollisionTriangleShape(TriangleShape3D<float>);
	CollisionTriangleShape(CollisionTriangleShape&&) noexcept = default;
	CollisionTriangleShape(const CollisionTriangleShape&) = delete;

	CollisionShape3D::ShapeType getShapeType() const override;
	const ConvexShape3D<float>& getSingleShape() const override;

	std::unique_ptr<CollisionShape3D> scale(const Vector3<float>&) const override;

	AABBox<float> toAABBox(const PhysicsTransform&) const override;
	std::unique_ptr<CollisionConvexObject3D, ObjectDeleter> toConvexObject(const PhysicsTransform&) const override;

	Vector3<float> computeLocalInertia(float) const override;
	float getMaxDistanceToCenter() const override;
	float getMinDistanceToCenter() const override;

	std::unique_ptr<CollisionShape3D> clone() const override;

private:
	TriangleShape3D<float> triangleShape; //shape including margin
};