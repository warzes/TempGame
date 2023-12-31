#include "stdafx.h"
#include "AIShape.h"

AIShape::AIShape(std::unique_ptr<ConvexShape3D<float>> shape) :
	shape(std::move(shape)),
	bHasLocalTransform(false) {

}

AIShape::AIShape(std::unique_ptr<ConvexShape3D<float>> shape, const nTransform<float>& localTransform) :
	shape(std::move(shape)),
	bHasLocalTransform(true),
	localTransform(localTransform) {

}

const ConvexShape3D<float>& AIShape::getShape() const {
	return *shape;
}

bool AIShape::hasLocalTransform() const {
	return bHasLocalTransform;
}

const nTransform<float>& AIShape::getLocalTransform() const {
	return localTransform;
}