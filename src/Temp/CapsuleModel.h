#pragma once

#include "GeometryModel.h"

class CapsuleModel final : public GeometryModel {
public:
	CapsuleModel(nCapsule<float>, unsigned int, unsigned int);

protected:
	std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const override;
	PrimitiveTopology getShapeType() const override;

private:
	void fillWithCaps(std::vector<Point3<float>>&, const Quaternion<float>&) const;

	nCapsule<float> capsule;
	unsigned int sides;
	unsigned int slices;
};