#pragma once

#include "NavTriangle.h"

struct NavPolygonEdge {
	std::shared_ptr<NavTriangle> triangle;
	std::size_t edgeIndex;
};