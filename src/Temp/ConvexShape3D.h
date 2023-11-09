#pragma once

#include "ConvexObject3D.h"
#include "Transform.h"

/**
* Represents a convex shape.
* A shape doesn't have a position in space.
*/
template<class T> class ConvexShape3D {
public:
	virtual ~ConvexShape3D() = default;

	virtual std::unique_ptr<ConvexShape3D<T>> clone() const = 0;
	virtual std::unique_ptr<ConvexObject3D<T>> toConvexObject(const nTransform<T>&) const = 0;
};