#pragma once

#include "CollisionAlgorithm.h"
#include "CollisionObjectWrapper.h"
#include "ManifoldResult.h"
#include "CollisionShape3D.h"
#include "FixedSizePool.h"

class CollisionAlgorithmBuilder {
public:
	virtual ~CollisionAlgorithmBuilder() = default;

	virtual CollisionAlgorithm* createCollisionAlgorithm(bool, const ManifoldResult&, FixedSizePool<CollisionAlgorithm>&) const = 0;

	virtual const std::vector<CollisionShape3D::ShapeType>& getFirstExpectedShapeType() const = 0;
	virtual unsigned int getAlgorithmSize() const = 0;
};