#pragma once

#include "GeometryModel.h"

class CylinderModel final : public GeometryModel {
public:
	CylinderModel(nCylinder<float>, unsigned int);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	nCylinder<float> cylinder;
	unsigned int sides;
};