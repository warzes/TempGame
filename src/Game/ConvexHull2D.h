#pragma once

#include "ConvexHullShape2D.h"
#include "ConvexObject2D.h"

/**
* Represents a convex hull 2D
*/
template<class T> class ConvexHull2D : public ConvexObject2D<T> {
public:
	template<class U> friend class ResizeConvexHull2DService;

	ConvexHull2D();
	explicit ConvexHull2D(const std::vector<Point2<T>>&);
	static std::unique_ptr<ConvexHull2D<T>> createFromCcwConvexPoints(const std::vector<Point2<T>>&);

	const std::vector<Point2<T>>& getPoints() const;

	Point2<T> getSupportPoint(const Vector2<T>&) const;
	T area() const;

	std::unique_ptr<ConvexHull2D<T>> resize(T) const;

private:
	ConvexHullShape2D<T> localizedConvexHullShape;
};