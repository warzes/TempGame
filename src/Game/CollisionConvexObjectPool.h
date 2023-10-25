#pragma once

#include "Common.h"
#include "SyncFixedSizePool.h"

class CollisionConvexObject3D;

class CollisionConvexObjectPool : public ThreadSafeSingleton<CollisionConvexObjectPool> {
public:
	friend class ThreadSafeSingleton<CollisionConvexObjectPool>;

	CollisionConvexObjectPool();
	~CollisionConvexObjectPool() override = default;

	SyncFixedSizePool<CollisionConvexObject3D>& getObjectsPool();

private:
	unsigned int maxObjectSize(const std::vector<unsigned int>&) const;

	std::unique_ptr<SyncFixedSizePool<CollisionConvexObject3D>> objectsPool;
};