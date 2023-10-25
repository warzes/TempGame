#pragma once

#include "PhysicsTransform.h"

class InertiaCalculation 
{
public:
	static Matrix3<float> computeInverseWorldInertia(const Vector3<float>&, const PhysicsTransform&);

private:
	InertiaCalculation() = default;
	~InertiaCalculation() = default;
};