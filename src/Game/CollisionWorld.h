#pragma once

#include "BodyContainer.h"
#include "ManifoldResult.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"
#include "IntegrateVelocity.h"
#include "ConstraintSolver.h"
#include "BodyActiveStateUpdater.h"
#include "IntegrateTransform.h"

/**
* Collision world: process integration, broad phase and narrow phase
*/
class CollisionWorld : public Observable {
public:
	explicit CollisionWorld(BodyContainer&);
	~CollisionWorld() override = default;

	enum NotificationType {
		COLLISION_RESULT_UPDATED
	};

	BroadPhase& getBroadPhase();
	NarrowPhase& getNarrowPhase();

	void process(float, const Vector3<float>&);

	const std::vector<ManifoldResult>& getLastUpdatedManifoldResults() const;

private:
	BodyContainer& bodyContainer;

	BroadPhase broadPhase;
	NarrowPhase narrowPhase;
	IntegrateVelocity integrateVelocity;
	ConstraintSolver constraintSolver;
	BodyActiveStateUpdater bodyActiveStateUpdater;
	IntegrateTransform integrateTransform;

	std::vector<ManifoldResult> manifoldResults;
};