#pragma once

#include "PhysicsWorld.h"
#include "Processable.h"
#include "RayTestResult.h"
#include "CollisionWorld.h"

class RayTester final : public Processable {
public:
	RayTester(CollisionWorld&, const nRay<float>& ray);

	std::shared_ptr<const RayTestResult> getRayTestResult() const;

	void setup(float, const Vector3<float>&) override;
	void execute(float, const Vector3<float>&) override;

private:
	CollisionWorld& collisionWorld;
	nRay<float> ray;

	std::shared_ptr<RayTestResult> rayTestResult;
};