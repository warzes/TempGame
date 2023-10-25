#pragma once

#include "LockById.h"

class ScopeLockById {
public:
	ScopeLockById(const std::shared_ptr<LockById>&, uint_fast32_t);
	~ScopeLockById();

private:
	std::shared_ptr<LockById> lockById;
	uint_fast32_t id;
};