#pragma once

#include "ConstraintSolvingData.h"
#include "CommonSolvingData.h"
#include "ImpulseSolvingData.h"
#include "BodyContainer.h"
#include "ManifoldResult.h"
#include "FixedSizePool.h"
#include "RigidBody.h"

class ConstraintSolver {
public:
	ConstraintSolver();
	~ConstraintSolver();

	void process(float, std::vector<ManifoldResult>&);

private:
	void setupConstraints(std::vector<ManifoldResult>&, float);
	void solveConstraints() const;

	CommonSolvingData fillCommonSolvingData(const ManifoldResult&, const ManifoldContactPoint&) const;
	ImpulseSolvingData fillImpulseSolvingData(const CommonSolvingData&, float) const;

	void solveNormalConstraint(const ConstraintSolvingData&) const;
	void solveTangentConstraint(const ConstraintSolvingData&) const;

	void applyImpulse(RigidBody&, RigidBody&, const CommonSolvingData&, const Vector3<float>&) const;
	Vector3<float> computeRelativeVelocity(const CommonSolvingData&) const;
	Vector3<float> computeTangent(const CommonSolvingData&, const Vector3<float>&) const;

	void logCommonData(std::string_view, const CommonSolvingData&) const;

	std::vector<ConstraintSolvingData*> constraintsSolvingData;
	std::unique_ptr<FixedSizePool<ConstraintSolvingData>> constraintSolvingDataPool;

	static constexpr unsigned int CONSTRAINT_SOLVER_ITERATION = 10;
	const float biasFactor;
	const bool useWarmStarting;
	const float restitutionVelocityThreshold;
};