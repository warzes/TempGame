#include "stdafx.h"
#include "SphereShape.h"
#include "Sphere.h"
#include "StringUtil.h"
#include "MathFunction.h"

template<class T> SphereShape<T>::SphereShape(T radius) :
	radius(radius) {
	assert(radius >= 0.0f);
}

template<class T> T SphereShape<T>::getRadius() const {
	return radius;
}

template<class T> std::unique_ptr<ConvexShape3D<T>> SphereShape<T>::clone() const {
	return std::make_unique<SphereShape<T>>(*this);
}

template<class T> std::unique_ptr<ConvexObject3D<T>> SphereShape<T>::toConvexObject(const nTransform<T>& transform) const {
	if (!MathFunction::isEqual((T)transform.getScale().X, (T)transform.getScale().Y, (T)0.01)
		|| !MathFunction::isEqual((T)transform.getScale().Y, (T)transform.getScale().Z, (T)0.01)) {
		throw std::runtime_error("Sphere cannot by transformed with different scales: " + StringUtil::toString(transform.getScale()));
	}

	T radiusScale = transform.getScale().X;
	return std::make_unique<nSphere<T>>(radius * radiusScale, transform.getPosition());
}

//explicit template
template class SphereShape<float>;
template class SphereShape<double>;