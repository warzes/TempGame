#pragma once

#include "Common.h"

class SplitBoundingBox {
public:
	SplitBoundingBox();

	void split(const AABBox<float>&, std::vector<AABBox<float>>&) const;

private:
	const float limitSize;
};