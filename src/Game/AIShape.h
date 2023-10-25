#pragma once

#include "Common.h"

class AIShape {
public:
	explicit AIShape(std::unique_ptr<ConvexShape3D<float>>);
	AIShape(std::unique_ptr<ConvexShape3D<float>>, const nTransform<float>& localTransform);

	const ConvexShape3D<float>& getShape() const;
	bool hasLocalTransform() const;
	const nTransform<float>& getLocalTransform() const;

private:
	std::unique_ptr<ConvexShape3D<float>> shape;

	bool bHasLocalTransform;
	nTransform<float> localTransform;
};