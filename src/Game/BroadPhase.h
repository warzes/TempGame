#pragma once

#include "BroadPhaseAlgorithm.h"
#include "PairContainer.h"
#include "AABBTreeAlgorithm.h"
#include "OverlappingPair.h"
#include "AbstractBody.h"
#include "BodyContainer.h"

/**
* Broad phase manager allowing to determine pairs of bodies potentially colliding
*/
class BroadPhase final : public Observer {
public:
	explicit BroadPhase(BodyContainer&);
	~BroadPhase() override;

	void notify(Observable*, int) override;

	void addBodyAsync(const std::shared_ptr<AbstractBody>&);
	void removeBodyAsync(const std::shared_ptr<AbstractBody>&);

	const std::vector<std::unique_ptr<OverlappingPair>>& computeOverlappingPairs();

	std::vector<std::shared_ptr<AbstractBody>> rayTest(const nRay<float>&) const;
	std::vector<std::shared_ptr<AbstractBody>> bodyTest(const AbstractBody&, const PhysicsTransform&, const PhysicsTransform&) const;

private:
	void addBody(const std::shared_ptr<AbstractBody>&);
	void removeBody(const AbstractBody&);
	void synchronizeBodies();

	AABBTreeAlgorithm broadPhaseAlgorithm;

	std::mutex mutex;
	std::vector<std::shared_ptr<AbstractBody>> bodiesToAdd;
	std::vector<std::shared_ptr<const AbstractBody>> bodiesToRemove;
};