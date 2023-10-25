#pragma once

#include "ConvexShape3D.h"

template<class T> class BoxShape final : public ConvexShape3D<T> {
public:
	BoxShape();
	explicit BoxShape(const Vector3<T>&);

	T getHalfSize(unsigned int) const;
	const Vector3<T>& getHalfSizes() const;
	T getMaxHalfSize() const;
	unsigned int getMaxHalfSizeIndex() const;
	T getMinHalfSize() const;
	unsigned int getMinHalfSizeIndex() const;

	Point3<T> getSupportPoint(const Vector3<T>&) const;
	T getVolume() const;

	std::unique_ptr<ConvexShape3D<T>> clone() const override;
	std::unique_ptr<ConvexObject3D<T>> toConvexObject(const nTransform<T>&) const override;

private:
	Vector3<T> halfSizes; //half sizes (width, height and depth) of box
};