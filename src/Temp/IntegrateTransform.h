#pragma once

#include "BodyContainer.h"
#include "RigidBody.h"
#include "BroadPhase.h"
#include "NarrowPhase.h"

/**
* Manager allowing to perform integration on bodies transformation
*/
class IntegrateTransform {
public:
	IntegrateTransform(const BodyContainer&, const BroadPhase&, const NarrowPhase&);

	void process(float) const;

private:
	void handleContinuousCollision(RigidBody&, const PhysicsTransform&, const PhysicsTransform&, float) const;

	static constexpr float MAX_LINEAR_VELOCITY_FACTOR = 0.95f;

	const BodyContainer& bodyContainer;
	const BroadPhase& broadPhase;
	const NarrowPhase& narrowPhase;
};