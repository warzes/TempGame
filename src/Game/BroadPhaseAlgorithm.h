#pragma once

#include "AbstractBody.h"
#include "OverlappingPair.h"
#include "PairContainer.h"

class BroadPhaseAlgorithm {
public:
	virtual ~BroadPhaseAlgorithm() = default;

	virtual void addBody(const std::shared_ptr<AbstractBody>&) = 0;
	virtual void removeBody(const AbstractBody&) = 0;
	virtual void updateBodies() = 0;

	virtual const std::vector<std::unique_ptr<OverlappingPair>>& getOverlappingPairs() const = 0;

	virtual std::vector<std::shared_ptr<AbstractBody>> rayTest(const nRay<float>&) const = 0;
	virtual std::vector<std::shared_ptr<AbstractBody>> bodyTest(const AbstractBody&, const PhysicsTransform&, const PhysicsTransform&) const = 0;
};