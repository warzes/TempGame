#pragma once

#include "AbstractBody.h"
#include "TemporalObject.h"
#include "ContinuousCollisionResult.h"

/**
* Implementation of GJK Ray Cast algorithm (see http://www.dtecta.com/papers/unpublished04raycast.pdf)
*/
template<class T, class U> class GJKContinuousCollisionAlgorithm {
public:
	ContinuousCollisionResult<U> calculateTimeOfImpact(const TemporalObject&, const TemporalObject&, std::shared_ptr<AbstractBody>) const;

private:
	Point3<T> getWorldSupportPoint(const TemporalObject&, const Vector3<T>&, const PhysicsTransform&) const;
	Point3<float> interpolate(const Point3<float>&, const Point3<float>&, T) const;

	void logInputData(const TemporalObject&, const TemporalObject&, std::string_view) const;

	static constexpr T SQUARE_EPSILON = std::numeric_limits<T>::epsilon() * std::numeric_limits<T>::epsilon();
	static constexpr unsigned int MAX_ITERATION = 25;
	static constexpr float TERMINATION_TOLERANCE = 0.00015f;
};