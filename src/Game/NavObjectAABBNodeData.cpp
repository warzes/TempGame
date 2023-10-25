#include "stdafx.h"
#include "NavObjectAABBNodeData.h"

NavObjectAABBNodeData::NavObjectAABBNodeData(const std::shared_ptr<NavObject>& navObject) :
	AABBNodeData(navObject) {

}

std::unique_ptr<AABBNodeData<std::shared_ptr<NavObject>>> NavObjectAABBNodeData::clone() const {
	throw std::logic_error("Clone not required for static polytope");
}

const std::string& NavObjectAABBNodeData::getObjectId() const {
	return getNodeObject()->getExpandedPolytope().getName();
}

AABBox<float> NavObjectAABBNodeData::retrieveObjectAABBox() const {
	return getNodeObject()->getExpandedPolytope().getAABBox();
}

bool NavObjectAABBNodeData::isObjectMoving() const {
	return false; //never move: always deleted and recreated
}