#pragma once

#include "GeometryModel.h"

class ConeModel final : public GeometryModel {
public:
	ConeModel(nCone<float>, unsigned int);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	nCone<float> cone;
	unsigned int slices;
};