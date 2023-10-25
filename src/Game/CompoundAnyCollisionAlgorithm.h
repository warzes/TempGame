#pragma once

#include "CollisionAlgorithm.h"
#include "CollisionAlgorithmBuilder.h"
#include "CollisionAlgorithmSelector.h"
#include "ManifoldResult.h"
#include "CollisionObjectWrapper.h"

class CompoundAnyCollisionAlgorithm final : public CollisionAlgorithm {
public:
	CompoundAnyCollisionAlgorithm(bool, const ManifoldResult&);
	~CompoundAnyCollisionAlgorithm() override = default;

	void doProcessCollisionAlgorithm(const CollisionObjectWrapper&, const CollisionObjectWrapper&) override;

	struct Builder : public CollisionAlgorithmBuilder {
		CollisionAlgorithm* createCollisionAlgorithm(bool, const ManifoldResult&, FixedSizePool<CollisionAlgorithm>&) const override;

		const std::vector<CollisionShape3D::ShapeType>& getFirstExpectedShapeType() const override;
		unsigned int getAlgorithmSize() const override;
	};

private:
	void addContactPointsToManifold(const ManifoldResult&, bool);
};