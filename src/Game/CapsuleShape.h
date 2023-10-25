#pragma once

#include "ConvexShape3D.h"

template<class T> class CapsuleShape final : public ConvexShape3D<T> {
public:
	enum CapsuleOrientation {
		CAPSULE_X,
		CAPSULE_Y,
		CAPSULE_Z
	};

	CapsuleShape(T, T, CapsuleOrientation);

	T getRadius() const;
	T getCylinderHeight() const;
	CapsuleOrientation getCapsuleOrientation() const;

	T computeHeight() const;

	std::unique_ptr<ConvexShape3D<T>> clone() const override;
	std::unique_ptr<ConvexObject3D<T>> toConvexObject(const nTransform<T>&) const override;

private:
	T radius;
	T cylinderHeight;
	CapsuleOrientation capsuleOrientation;
};