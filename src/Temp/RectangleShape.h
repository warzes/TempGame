#pragma once

#include "ConvexShape2D.h"
#include "Vector2.h"

/**
* Represents a rectangle aligned on axis
*/
template<class T> class RectangleShape : public ConvexShape2D {
public:
	explicit RectangleShape(const Vector2<T>&);

	const Vector2<T>& getDiagonal() const;

private:
	Vector2<T> diagonal; //diagonal from minimum to maximum point
};