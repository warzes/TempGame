#pragma once

#include "GeometryModel.h"

class OBBoxModel final : public GeometryModel {
public:
	explicit OBBoxModel(OBBox<float>);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	OBBox<float> obbox;
};