#pragma once

#include "ConvexHullShape3D.h"
#include "ConvexHull3D.h"
#include "Plane.h"
#include "Point3.h"

template<class T> class ResizeConvexHull3DService {
public:
	ResizeConvexHull3DService() = delete;

	static std::unique_ptr<ConvexHullShape3D<T>> resizeConvexHullShape(const ConvexHullShape3D<T>&, T);
	static std::unique_ptr<ConvexHull3D<T>> resizeConvexHull(const ConvexHull3D<T>&, T);

	static std::unique_ptr<ConvexHullShape3D<T>> resizeConvexHullShape(const ConvexHullShape3D<T>&, const std::map<std::size_t, nPlane<T>>&);
	static std::unique_ptr<ConvexHull3D<T>> resizeConvexHull(const ConvexHull3D<T>&, const std::map<std::size_t, nPlane<T>>&);

private:
	static std::map<std::size_t, nPlane<T>> buildPlanesFromConvexHullShape(const ConvexHullShape3D<T>&);
	static void shiftPlanes(std::map<std::size_t, nPlane<T>>&, T);
	static std::vector<nPlane<T>> findThreeNonParallelPlanes(const std::vector<std::size_t>&, const std::map<std::size_t, nPlane<T>>&);
};