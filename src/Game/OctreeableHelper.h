#pragma once

template<class T> class OctreeableHelper {
public:
	OctreeableHelper() = delete;
	~OctreeableHelper() = delete;

	static void merge(std::vector<T*>&, std::span<T* const>);
};

template<class T> void OctreeableHelper<T>::merge(std::vector<T*>& targetOctreeables, std::span<T* const> additionalOctreeables) {
	std::ranges::for_each(targetOctreeables, [](T* o) { o->setProcessed(true); });

	for (auto& additionalOctreeable : additionalOctreeables) {
		if (!additionalOctreeable->isProcessed()) {
			targetOctreeables.emplace_back(additionalOctreeable);
		}
	}

	std::ranges::for_each(targetOctreeables, [](T* o) { o->setProcessed(false); });
}
