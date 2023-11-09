#pragma once

#include "RigidBody.h"
#include "ManifoldContactPoint.h"
#include "CommonSolvingData.h"
#include "ImpulseSolvingData.h"
#include "AccumulatedSolvingData.h"

class ConstraintSolvingData {
public:
	ConstraintSolvingData(RigidBody&, RigidBody&, ManifoldContactPoint&, const CommonSolvingData&, const ImpulseSolvingData&);

	RigidBody& getBody1() const;
	RigidBody& getBody2() const;

	const CommonSolvingData& getCommonData() const;
	const ImpulseSolvingData& getImpulseData() const;
	AccumulatedSolvingData& getAccumulatedData() const;

private:
	RigidBody& body1;
	RigidBody& body2;
	ManifoldContactPoint& manifoldContactPoint;

	CommonSolvingData commonData;
	ImpulseSolvingData impulseData;
};