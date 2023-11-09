#pragma once

#include "ConvexShape3D.h"

template<class T> class SphereShape final : public ConvexShape3D<T> {
public:
	explicit SphereShape(T);

	T getRadius() const;

	std::unique_ptr<ConvexShape3D<T>> clone() const override;
	std::unique_ptr<ConvexObject3D<T>> toConvexObject(const nTransform<T>&) const override;

private:
	T radius;
};