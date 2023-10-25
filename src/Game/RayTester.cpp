#include "stdafx.h"
#include "RayTester.h"
#include "ContinuousCollisionResult.h"

RayTester::RayTester(CollisionWorld& collisionWorld, const nRay<float>& ray) :
	collisionWorld(collisionWorld),
	ray(ray),
	rayTestResult(std::make_shared<RayTestResult>()) {

}

std::shared_ptr<const RayTestResult> RayTester::getRayTestResult() const {
	return rayTestResult;
}

void RayTester::setup(float, const Vector3<float>&) {
	//nothing to do
}

void RayTester::execute(float, const Vector3<float>&) {
	std::vector<std::shared_ptr<AbstractBody>> bodiesAABBoxHitRay = collisionWorld.getBroadPhase().rayTest(ray);
	ccd_set rayCastResults = collisionWorld.getNarrowPhase().rayTest(ray, bodiesAABBoxHitRay);

	rayTestResult->addResults(rayCastResults);
}