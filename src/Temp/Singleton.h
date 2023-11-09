#pragma once

#include "SingletonContainer.h"
#include "SingletonInterface.h"

/**
* Base class for singleton (no thread-safe)
*/
template<class T> class Singleton : public SingletonInterface {
public:
	static T& instance();
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	~Singleton() override;

protected:
	Singleton() = default;

private:
	static T* objectT;
};

//static
template<class T> T* Singleton<T>::objectT = nullptr;

template<class T> Singleton<T>::~Singleton() {
	objectT = nullptr;
}

template<class T> T& Singleton<T>::instance() {
	if (!objectT) {
		auto newObjectT = std::unique_ptr<T>(new T);
		objectT = newObjectT.get();
		SingletonContainer::registerSingleton(typeid(T).name(), std::move(newObjectT));
	}

	return *objectT;
}