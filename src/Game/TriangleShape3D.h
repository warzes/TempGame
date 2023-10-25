#pragma once

#include "ConvexShape3D.h"

template<class T> class TriangleShape3D final : public ConvexShape3D<T> {
public:
	explicit TriangleShape3D(const std::array<Point3<T>, 3>&);
	TriangleShape3D(const Point3<T>&, const Point3<T>&, const Point3<T>&);

	const std::array<Point3<T>, 3>& getPoints() const;

	std::unique_ptr<ConvexShape3D<T>> clone() const override;
	std::unique_ptr<ConvexObject3D<T>> toConvexObject(const nTransform<T>&) const override;

private:
	std::array<Point3<T>, 3> points;
};