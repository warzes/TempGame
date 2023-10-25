#pragma once

#include "AbstractBody.h"
#include "CollisionAlgorithm.h"
#include "CollisionAlgorithmBuilder.h"
#include "SyncFixedSizePool.h"

class AlgorithmDeleter {
public:
	AlgorithmDeleter() = default;
	explicit AlgorithmDeleter(std::shared_ptr<FixedSizePool<CollisionAlgorithm>>);
	void operator()(CollisionAlgorithm*) const;

private:
	std::shared_ptr<FixedSizePool<CollisionAlgorithm>> algorithmPool;
};

class CollisionAlgorithmSelector {
public:
	CollisionAlgorithmSelector();

	std::unique_ptr<CollisionAlgorithm, AlgorithmDeleter> createCollisionAlgorithm(
		AbstractBody&, const CollisionShape3D&, AbstractBody&, const CollisionShape3D&) const;

private:
	void initializeCollisionAlgorithmBuilderMatrix();
	void initializeConcaveAlgorithm();
	void initializeCompoundAlgorithm();

	void initializeAlgorithmPool();

	std::shared_ptr<SyncFixedSizePool<CollisionAlgorithm>> algorithmPool;
	std::array<std::array<std::unique_ptr<CollisionAlgorithmBuilder>, CollisionShape3D::SHAPE_MAX>, CollisionShape3D::SHAPE_MAX> collisionAlgorithmBuilderMatrix;
};