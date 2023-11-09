#pragma once

#include "Common.h"
#include "Resource.h"

class ResourceContainer final : public ThreadSafeSingleton<ResourceContainer> {
public:
	friend class ThreadSafeSingleton<ResourceContainer>;

	~ResourceContainer() override;

	template<class T> std::shared_ptr<T> getResource(std::string_view) const;
	void addResource(const std::shared_ptr<Resource>&);
	void cleanResources();

private:
	ResourceContainer();

	void cleanResources(bool);

	mutable std::mutex mutex;
	std::map<std::string, std::shared_ptr<Resource>, std::less<>> resources;
};

template<class T> std::shared_ptr<T> ResourceContainer::getResource(std::string_view resourceId) const {
	std::scoped_lock<std::mutex> lock(mutex);

	auto itFind = resources.find(resourceId);
	if (itFind != resources.end()) {
		return std::dynamic_pointer_cast<T>(itFind->second);
	}
	return std::shared_ptr<T>(nullptr);
}