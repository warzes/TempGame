#pragma once

#include "AbstractBody.h"
#include "OverlappingPair.h"
#include "BroadPhaseAlgorithm.h"
#include "BodyAABBTree.h"

class AABBTreeAlgorithm final : public BroadPhaseAlgorithm {
public:
	AABBTreeAlgorithm();

	void addBody(const std::shared_ptr<AbstractBody>&) override;
	void removeBody(const AbstractBody&) override;
	void updateBodies() override;

	const std::vector<std::unique_ptr<OverlappingPair>>& getOverlappingPairs() const override;

	std::vector<std::shared_ptr<AbstractBody>> rayTest(const nRay<float>&) const override;
	std::vector<std::shared_ptr<AbstractBody>> bodyTest(const AbstractBody&, const PhysicsTransform&, const PhysicsTransform&) const override;

private:
	BodyAABBTree tree;
};