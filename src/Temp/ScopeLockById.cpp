#include "stdafx.h"
#include "ScopeLockById.h"

ScopeLockById::ScopeLockById(const std::shared_ptr<LockById>& lockById, uint_fast32_t id) :
	lockById(lockById),
	id(id) {
	lockById->lock(id);
}

ScopeLockById::~ScopeLockById() {
	lockById->unlock(id);
}