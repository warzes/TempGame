#include "stdafx.h"
#include "ObjectDeleter.h"
#include "CollisionConvexObject3D.h"
#include "CollisionConvexObjectPool.h"


void ObjectDeleter::operator()(CollisionConvexObject3D* const collisionAlgorithm) const {
	CollisionConvexObjectPool::instance().getObjectsPool().deallocate(collisionAlgorithm);
}