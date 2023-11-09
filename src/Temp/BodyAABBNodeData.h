#pragma once

#include "AbstractBody.h"
#include "PairContainer.h"

class BodyAABBNodeData final : public AABBNodeData<std::shared_ptr<AbstractBody>> {
public:
	explicit BodyAABBNodeData(const std::shared_ptr<AbstractBody>&);

	std::unique_ptr<AABBNodeData<std::shared_ptr<AbstractBody>>> clone() const override;

	const std::string& getObjectId() const override;
	AABBox<float> retrieveObjectAABBox() const override;
	bool isObjectMoving() const override;

	bool isGhostBody() const;
	PairContainer* getBodyPairContainer() const;

	void addOwnerPairContainer(PairContainer*);
	void removeOwnerPairContainer(PairContainer*);
	std::set<PairContainer*> getOwnerPairContainers() const;

private:
	PairContainer* bodyPairContainer;
	std::set<PairContainer*> ownerPairContainers;
};