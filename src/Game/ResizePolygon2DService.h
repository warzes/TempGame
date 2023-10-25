#pragma once

#include "Point2.h"

template<class T> class ResizePolygon2DService {
public:
	ResizePolygon2DService() = delete;

	static void resizePolygon(std::vector<Point2<T>>&, T);
};