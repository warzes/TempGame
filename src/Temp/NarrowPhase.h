#pragma once

#include "Common.h"
#include "ManifoldResult.h"
#include "OverlappingPair.h"
#include "CollisionAlgorithm.h"
#include "CollisionAlgorithmSelector.h"
#include "GJKContinuousCollisionAlgorithm.h"
#include "BroadPhase.h"
#include "BodyContainer.h"
#include "AbstractBody.h"
#include "GhostBody.h"
#include "TemporalObject.h"
#include "CollisionTriangleShape.h"

class NarrowPhase {
public:
	NarrowPhase(const BodyContainer&, const BroadPhase&);

	void process(float, const std::vector<std::unique_ptr<OverlappingPair>>&, std::vector<ManifoldResult>&);
	void processGhostBody(const GhostBody&, std::vector<ManifoldResult>&);

	ccd_set continuousCollisionTest(const TemporalObject&, const std::vector<std::shared_ptr<AbstractBody>>&) const;
	ccd_set rayTest(const nRay<float>&, const std::vector<std::shared_ptr<AbstractBody>>&) const;

private:
	void processOverlappingPairs(const std::vector<std::unique_ptr<OverlappingPair>>&, std::vector<ManifoldResult>&);
	void processOverlappingPair(OverlappingPair&, std::vector<ManifoldResult>&);
	CollisionAlgorithm* retrieveCollisionAlgorithm(OverlappingPair&);

	void processPredictiveContacts(float, std::vector<ManifoldResult>&) const;
	void handleContinuousCollision(AbstractBody&, const PhysicsTransform&, const PhysicsTransform&, std::vector<ManifoldResult>&) const;
	void trianglesContinuousCollisionTest(const std::vector<CollisionTriangleShape>&, const TemporalObject&, const std::shared_ptr<AbstractBody>&, ccd_set&) const;
	void continuousCollisionTest(const TemporalObject&, const TemporalObject&, std::shared_ptr<AbstractBody>, ccd_set&) const;

	const BodyContainer& bodyContainer;
	const BroadPhase& broadPhase;

	CollisionAlgorithmSelector collisionAlgorithmSelector;
	GJKContinuousCollisionAlgorithm<double, float> gjkContinuousCollisionAlgorithm;

	std::shared_ptr<LockById> bodiesMutex;

	static thread_local std::vector<OverlappingPair> overlappingPairsCache;
};