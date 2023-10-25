#pragma once

#include "GeometryModel.h"

class SphereModel final : public GeometryModel {
public:
	SphereModel(nSphere<float>, unsigned int);
	SphereModel(std::vector<nSphere<float>>, unsigned int);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	std::vector<nSphere<float>> spheres;
	unsigned int slices;
};