#pragma once

#include "GeometryModel.h"

class AABBoxModel final : public GeometryModel {
public:
	explicit AABBoxModel(const AABBox<float>&);
	explicit AABBoxModel(std::vector<AABBox<float>>);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	std::vector<AABBox<float>> aabboxes;
};