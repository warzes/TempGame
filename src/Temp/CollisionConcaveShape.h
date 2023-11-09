#pragma once

#include "PhysicsTransform.h"
#include "CollisionTriangleShape.h"

class CollisionConcaveShape {
public:
	virtual ~CollisionConcaveShape() = default;

	virtual const std::vector<CollisionTriangleShape>& findTrianglesInAABBox(const AABBox<float>&) const = 0;
	virtual const std::vector<CollisionTriangleShape>& findTrianglesHitByRay(const LineSegment3D<float>&) const = 0;
};