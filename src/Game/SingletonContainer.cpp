#include "stdafx.h"
#include "SingletonContainer.h"

//static
std::vector<SingletonData> SingletonContainer::singletonsVector;

void SingletonContainer::registerSingleton(const std::string& name, std::unique_ptr<SingletonInterface> ptr) {
#ifdef _DEBUG
	assert(std::ranges::count_if(singletonsVector, [&name](const SingletonData& sa) { return sa.name == name; }) == 0);
#endif
	singletonsVector.emplace_back(SingletonData{ .name = name, .ptr = std::move(ptr) });
}

void SingletonContainer::destroyAllSingletons() {
	for (int i = (int)singletonsVector.size() - 1; i >= 0; --i) {
		singletonsVector.resize((std::size_t)i); //destroy singletons in FILO
	}
}