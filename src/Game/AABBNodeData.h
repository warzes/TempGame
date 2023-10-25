#pragma once

#include "AABBox.h"

template<class OBJ> class AABBNodeData {
public:
	explicit AABBNodeData(OBJ);
	virtual ~AABBNodeData() = default;

	const OBJ& getNodeObject() const;

	virtual std::unique_ptr<AABBNodeData<OBJ>> clone() const = 0;

	virtual const std::string& getObjectId() const = 0;
	virtual AABBox<float> retrieveObjectAABBox() const = 0;
	virtual bool isObjectMoving() const = 0;

private:
	OBJ nodeObject;
};

template<class OBJ> AABBNodeData<OBJ>::AABBNodeData(OBJ nodeObject) :
	nodeObject(nodeObject) {

}

template<class OBJ> const OBJ& AABBNodeData<OBJ>::getNodeObject() const {
	return nodeObject;
}