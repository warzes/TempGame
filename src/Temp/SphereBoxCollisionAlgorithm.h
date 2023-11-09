#pragma once

#include "CollisionAlgorithm.h"
#include "CollisionAlgorithmBuilder.h"
#include "ManifoldResult.h"
#include "CollisionObjectWrapper.h"

class SphereBoxCollisionAlgorithm final : public CollisionAlgorithm {
public:
	SphereBoxCollisionAlgorithm(bool, const ManifoldResult&);
	~SphereBoxCollisionAlgorithm() override = default;

	void doProcessCollisionAlgorithm(const CollisionObjectWrapper&, const CollisionObjectWrapper&) override;

	struct Builder : public CollisionAlgorithmBuilder {
		CollisionAlgorithm* createCollisionAlgorithm(bool, const ManifoldResult&, FixedSizePool<CollisionAlgorithm>&) const override;

		const std::vector<CollisionShape3D::ShapeType>& getFirstExpectedShapeType() const override;
		unsigned int getAlgorithmSize() const override;
	};
};