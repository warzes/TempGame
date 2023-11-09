#pragma once

#include "Common.h"
#include "NavObject.h"

class NavObjectAABBNodeData final : public AABBNodeData<std::shared_ptr<NavObject>> {
public:
	explicit NavObjectAABBNodeData(const std::shared_ptr<NavObject>&);

	std::unique_ptr<AABBNodeData<std::shared_ptr<NavObject>>> clone() const override;

	const std::string& getObjectId() const override;
	AABBox<float> retrieveObjectAABBox() const override;
	bool isObjectMoving() const override;
};