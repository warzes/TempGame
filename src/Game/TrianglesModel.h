#pragma once

#include "GeometryModel.h"

class TrianglesModel final : public GeometryModel {
public:
	explicit TrianglesModel(std::vector<Point3<float>>);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	std::vector<Point3<float>> trianglesPoints;
};