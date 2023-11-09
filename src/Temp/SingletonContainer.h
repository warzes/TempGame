#pragma once

#include "SingletonInterface.h"

struct SingletonData {
	std::string name;
	std::unique_ptr<SingletonInterface> ptr;
};

class SingletonContainer {
public:
	static void registerSingleton(const std::string&, std::unique_ptr<SingletonInterface>);

	static void destroyAllSingletons();

private:
	SingletonContainer() = default;
	~SingletonContainer() = default;

	static std::vector<SingletonData> singletonsVector;
};