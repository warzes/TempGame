#pragma once

#include "SingletonContainer.h"
#include "SingletonInterface.h"

/**
* Base class for thread-safe singleton
*/
template<class T> class ThreadSafeSingleton : public SingletonInterface {
public:
	static T& instance();
	ThreadSafeSingleton(const ThreadSafeSingleton&) = delete;
	ThreadSafeSingleton& operator=(const ThreadSafeSingleton&) = delete;
	~ThreadSafeSingleton() override;

protected:
	ThreadSafeSingleton() = default;

private:
	static std::atomic<T*> objectT;
	static std::mutex mutexInstanceCreation;
};

//static
template<class T> std::atomic<T*> ThreadSafeSingleton<T>::objectT = nullptr;
template<class T> std::mutex ThreadSafeSingleton<T>::mutexInstanceCreation;

template<class T> ThreadSafeSingleton<T>::~ThreadSafeSingleton() {
	objectT.store(nullptr, std::memory_order_release);
}

template<class T> T& ThreadSafeSingleton<T>::instance() {
	T* singletonInstance = objectT.load(std::memory_order_acquire);
	if (!singletonInstance) [[unlikely]] {
		std::scoped_lock<std::mutex> lock(mutexInstanceCreation);
		singletonInstance = objectT.load(std::memory_order_relaxed);
		if (!singletonInstance) {
			auto newObjectT = std::unique_ptr<T>(new T);
			singletonInstance = newObjectT.get();
			SingletonContainer::registerSingleton(typeid(T).name(), std::move(newObjectT));
			objectT.store(singletonInstance, std::memory_order_release);
		}
		}

	return *singletonInstance;
}
