#pragma once

#include "ConvexObject2D.h"
#include "Point2.h"
#include "RectangleShape.h"

/**
* Represents a rectangle aligned on axis in space
*/
template<class T> class Rectangle2D : public ConvexObject2D<T> {
public:
	Rectangle2D();
	Rectangle2D(const Point2<T>&, const Vector2<T>&);
	Rectangle2D(const Point2<T>&, const Point2<T>&);

	Point2<T> getMin() const;
	Point2<T> getMax() const;
	Vector2<T> getDiagonal() const;
	Point2<T> getSupportPoint(const Vector2<T>&) const;

	Rectangle2D<T> merge(const Rectangle2D<T>&) const;

	bool collideWithPoint(const Point2<T>&) const;
	bool collideWithRectangle(const Rectangle2D<T>&) const;

private:
	RectangleShape<T> rectangleShape;
	Point2<T> origin; //origin of rectangle (minimum point on X and Y axis)
};