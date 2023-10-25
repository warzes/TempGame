#include "stdafx.h"
#include "CollisionConvexObjectPool.h"
#include "CollisionConvexObject3D.h"
#include "CollisionBoxObject.h"
#include "CollisionCapsuleObject.h"
#include "CollisionConeObject.h"
#include "CollisionConvexHullObject.h"
#include "CollisionCylinderObject.h"
#include "CollisionSphereObject.h"
#include "CollisionTriangleObject.h"

CollisionConvexObjectPool::CollisionConvexObjectPool() {
	unsigned int maxElementSize = maxObjectSize({
	sizeof(CollisionBoxObject),
	sizeof(CollisionCapsuleObject),
	sizeof(CollisionConeObject),
	sizeof(CollisionConvexHullObject),
	sizeof(CollisionCylinderObject),
	sizeof(CollisionSphereObject),
	sizeof(CollisionTriangleObject)
		});
	//unsigned int objectsPoolSize = ConfigService::instance().getUnsignedIntValue("collisionObject.poolSize");
	unsigned int objectsPoolSize = 8192;

	//pool is synchronized because elements are created in narrow phase (= synchronized phase called by different threads) and deleted by different threads outside the narrow phase
	objectsPool = std::make_unique<SyncFixedSizePool<CollisionConvexObject3D>>("collisionConvexObjectsPool", maxElementSize, objectsPoolSize);
}

SyncFixedSizePool<CollisionConvexObject3D>& CollisionConvexObjectPool::getObjectsPool() {
	return *objectsPool;
}

unsigned int CollisionConvexObjectPool::maxObjectSize(const std::vector<unsigned int>& objectsSize) const {
	unsigned int result = 0;
	for (unsigned int objectSize : objectsSize) {
		result = std::max(result, objectSize);
	}
	return result;
}