#pragma once

#include "TaggableResource.h"

class TagHolder {
public:
	void addTaggableResource(TaggableResource&);
	void removeTaggableResource(TaggableResource&) const;

	template<class T> void findByTag(std::string_view, std::vector<T>&) const;
	void addTag(TaggableResource*, std::string);
	void removeTag(const TaggableResource*, std::string_view);

private:
	std::multimap<std::string, TaggableResource*, std::less<>> tagsMap;
};

/**
* @param result [out] All resources having the requested tag
*/
template<class T> void TagHolder::findByTag(std::string_view tag, std::vector<T>& result) const {
	const auto& [lowerBound, upperBound] = tagsMap.equal_range(tag);
	for (auto it = lowerBound; it != upperBound; ++it) {
		result.push_back(static_cast<T>(it->second));
	}
}