#pragma once

#include "CollisionAlgorithm.h"
#include "CollisionAlgorithmBuilder.h"
#include "ManifoldResult.h"
#include "CollisionObjectWrapper.h"
#include "EPAAlgorithm.h"

class ConvexConvexCollisionAlgorithm final : public CollisionAlgorithm {
public:
	ConvexConvexCollisionAlgorithm(bool, const ManifoldResult&);
	~ConvexConvexCollisionAlgorithm() override = default;

	void doProcessCollisionAlgorithm(const CollisionObjectWrapper&, const CollisionObjectWrapper&) override;

	struct Builder : public CollisionAlgorithmBuilder {
		CollisionAlgorithm* createCollisionAlgorithm(bool, const ManifoldResult&, FixedSizePool<CollisionAlgorithm>&) const override;

		const std::vector<CollisionShape3D::ShapeType>& getFirstExpectedShapeType() const override;
		unsigned int getAlgorithmSize() const override;
	};

private:
	void processCollisionAlgorithmWithMargin(const CollisionConvexObject3D&, const CollisionConvexObject3D&);

	GJKAlgorithm<double> gjkAlgorithm;
	EPAAlgorithm<double> epaAlgorithm;
};