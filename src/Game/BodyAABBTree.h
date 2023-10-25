#pragma once

#include "AbstractBody.h"
#include "OverlappingPair.h"
#include "PairContainer.h"
#include "BroadPhaseAlgorithm.h"
#include "VectorPairContainer.h"
#include "BodyAABBNodeData.h"

class BodyAABBTree final : public AABBTree<std::shared_ptr<AbstractBody>> {
public:
	BodyAABBTree();
	~BodyAABBTree() override = default;

	void addBody(const std::shared_ptr<AbstractBody>&);
	void postAddObjectCallback(AABBNode<std::shared_ptr<AbstractBody>>&) override;

	void removeBody(const AbstractBody&);
	void preRemoveObjectCallback(AABBNode<std::shared_ptr<AbstractBody>>&) override;

	void updateBodies();
	void preUpdateObjectCallback(AABBNode<std::shared_ptr<AbstractBody>>&) override;

	const std::vector<std::unique_ptr<OverlappingPair>>& getOverlappingPairs() const;

private:
	void computeOverlappingPairs(const AABBNode<std::shared_ptr<AbstractBody>>&);
	void createOverlappingPair(BodyAABBNodeData&, BodyAABBNodeData&);
	void removeOverlappingPairs(const BodyAABBNodeData&);
	void removeBodyPairContainerReferences(const AbstractBody&, PairContainer*) const;

	void computeWorldBoundary();
	void controlBoundaries(const AABBNode<std::shared_ptr<AbstractBody>>&) const;

	static constexpr float BOUNDARIES_MARGIN_PERCENTAGE = 0.3f;

	VectorPairContainer defaultPairContainer;

	bool inInitializationPhase;
	float minYBoundary;
};