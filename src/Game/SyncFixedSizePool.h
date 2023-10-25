#pragma once

#include "FixedSizePool.h"

template<class BaseType> class SyncFixedSizePool final : public FixedSizePool<BaseType> {
public:
	SyncFixedSizePool(const std::string&, unsigned int, unsigned int);
	~SyncFixedSizePool() = default;

	void* allocate(unsigned int) override;
	void deallocate(BaseType*) override;

private:
	std::mutex mutex;
};

template<class BaseType> SyncFixedSizePool<BaseType>::SyncFixedSizePool(const std::string& poolName, unsigned int maxElementSize, unsigned int maxElements) :
	FixedSizePool<BaseType>(poolName, maxElementSize, maxElements) {

}

template<class BaseType> void* SyncFixedSizePool<BaseType>::allocate(unsigned int size) {
	std::scoped_lock<std::mutex> lock(mutex);

	return FixedSizePool<BaseType>::allocate(size);
}

template<class BaseType> void SyncFixedSizePool<BaseType>::deallocate(BaseType* ptr) {
	std::scoped_lock<std::mutex> lock(mutex);

	FixedSizePool<BaseType>::deallocate(ptr);
}
