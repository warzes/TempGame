#pragma once

#include "Common.h"
#include "AIEntity.h"
#include "AIShape.h"

class AIObject final : public AIEntity {
public:
	AIObject(std::string, const nTransform<float>&, bool, std::unique_ptr<AIShape>);
	AIObject(std::string, const nTransform<float>&, bool, std::vector<std::unique_ptr<AIShape>>);

	AIEntity::AIEntityType getType() const override;

	const std::vector<std::unique_ptr<AIShape>>& getShapes() const;

private:
	std::vector<std::unique_ptr<AIShape>> shapes;
};