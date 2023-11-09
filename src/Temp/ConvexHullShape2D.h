#pragma once

#include "ConvexShape2D.h"
#include "Point2.h"

template<class T> class ConvexHullShape2D : public ConvexShape2D {
public:
	ConvexHullShape2D() = default;
	explicit ConvexHullShape2D(const std::vector<Point2<T>>&);
	~ConvexHullShape2D() override = default;
	static std::unique_ptr<ConvexHullShape2D<T>> createFromCcwConvexPoints(const std::vector<Point2<T>>&);

	const std::vector<Point2<T>>& getPoints() const;

	Point2<T> getSupportPoint(const Vector2<T>&) const;
	T area() const;

	std::unique_ptr<ConvexHullShape2D<T>> resize(T) const;

private:
	std::vector<Point2<T>> convexHullPoints;

};