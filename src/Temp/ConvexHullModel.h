#pragma once

#include "GeometryModel.h"

class ConvexHullModel final : public GeometryModel {
public:
	explicit ConvexHullModel(ConvexHull3D<float>);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	ConvexHull3D<float> convexHull;
};