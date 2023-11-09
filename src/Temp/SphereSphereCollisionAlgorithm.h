#pragma once

#include "CollisionAlgorithm.h"
#include "CollisionAlgorithmBuilder.h"
#include "ManifoldResult.h"
#include "CollisionObjectWrapper.h"

class SphereSphereCollisionAlgorithm final : public CollisionAlgorithm {
public:
	SphereSphereCollisionAlgorithm(bool, const ManifoldResult&);
	~SphereSphereCollisionAlgorithm() override = default;

	void doProcessCollisionAlgorithm(const CollisionObjectWrapper&, const CollisionObjectWrapper&) override;

	struct Builder : public CollisionAlgorithmBuilder {
		CollisionAlgorithm* createCollisionAlgorithm(bool, const ManifoldResult&, FixedSizePool<CollisionAlgorithm>&) const override;

		const std::vector<CollisionShape3D::ShapeType>& getFirstExpectedShapeType() const override;
		unsigned int getAlgorithmSize() const override;
	};
};