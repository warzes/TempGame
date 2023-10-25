#pragma once

#include "ConvexHullShape2D.h"
#include "ConvexHull2D.h"

template<class T> class ResizeConvexHull2DService {
public:
	ResizeConvexHull2DService() = delete;

	static std::unique_ptr<ConvexHullShape2D<T>> resizeConvexHullShape(const ConvexHullShape2D<T>&, T);
	static std::unique_ptr<ConvexHull2D<T>> resizeConvexHull(const ConvexHull2D<T>&, T);
};