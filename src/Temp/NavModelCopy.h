#pragma once

#include "NavPolygon.h"

class NavModelCopy {
public:
	static void copyNavPolygons(const std::vector<std::shared_ptr<NavPolygon>>&, std::vector<std::shared_ptr<NavPolygon>>&);
};