#pragma once

#include "Common.h"
#include "AIEntity.h"

class AITerrain final : public AIEntity {
public:
	AITerrain(std::string, const nTransform<float>&, bool, std::vector<Point3<float>>, unsigned int, unsigned int);

	AIEntity::AIEntityType getType() const override;

	const std::vector<Point3<float>>& getLocalVertices() const;
	unsigned int getXLength() const;
	unsigned int getZLength() const;

private:
	std::vector<Point3<float>> localVertices;
	unsigned int xLength;
	unsigned int zLength;
};