#pragma once

#include "Point2.h"

template<class T> class ConvexObject2D 
{
public:
	virtual ~ConvexObject2D() = default;
	virtual Point2<T> getSupportPoint(const Vector2<T>&) const = 0;
};