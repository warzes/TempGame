#pragma once

#include "AABBNodeData.h"
#include "AABBox.h"

template<class OBJ> class AABBTree;

template<class OBJ> class AABBNode {
public:
	friend class AABBTree<OBJ>;

	explicit AABBNode(std::unique_ptr<AABBNodeData<OBJ>>);

	AABBNodeData<OBJ>& getNodeData() const;

	bool isLeaf() const;
	bool isRoot() const;

	AABBNode<OBJ>* getLeftChild() const;
	AABBNode<OBJ>* getRightChild() const;

	const AABBox<float>& getAABBox() const;

protected:
	void updateAABBox(float);

	std::unique_ptr<AABBNodeData<OBJ>> moveNodeData();

	void setParent(AABBNode<OBJ>*);
	AABBNode<OBJ>* getParent() const;

	void setLeftChild(std::shared_ptr<AABBNode<OBJ>>);
	std::shared_ptr<AABBNode<OBJ>> getLeftChildSmartPtr() const;
	void setRightChild(std::shared_ptr<AABBNode<OBJ>>);
	std::shared_ptr<AABBNode<OBJ>> getRightChildSmartPtr() const;

	std::shared_ptr<AABBNode<OBJ>> getSibling() const;

private:
	std::unique_ptr<AABBNodeData<OBJ>> nodeData;
	AABBox<float> aabbox;

	AABBNode<OBJ>* parentNode;
	std::array<std::shared_ptr<AABBNode<OBJ>>, 2> children;
};

template<class OBJ> AABBNode<OBJ>::AABBNode(std::unique_ptr<AABBNodeData<OBJ>> nodeData) :
	nodeData(std::move(nodeData)),
	parentNode(nullptr) {

}

template<class OBJ> AABBNodeData<OBJ>& AABBNode<OBJ>::getNodeData() const {
	assert(nodeData);
	return *nodeData;
}

template<class OBJ> std::unique_ptr<AABBNodeData<OBJ>> AABBNode<OBJ>::moveNodeData() {
	return std::move(nodeData);
}

template<class OBJ> bool AABBNode<OBJ>::isLeaf() const {
	return children[0] == nullptr;
}

template<class OBJ> bool AABBNode<OBJ>::isRoot() const {
	return parentNode == nullptr;
}

template<class OBJ> void AABBNode<OBJ>::setParent(AABBNode<OBJ>* parentNode) {
	this->parentNode = parentNode;
}

template<class OBJ> AABBNode<OBJ>* AABBNode<OBJ>::getParent() const {
	return parentNode;
}

template<class OBJ> void AABBNode<OBJ>::setLeftChild(std::shared_ptr<AABBNode<OBJ>> leftChild) {
	children[0] = leftChild;
	if (leftChild) {
		children[0]->setParent(this);
	}
}

template<class OBJ> AABBNode<OBJ>* AABBNode<OBJ>::getLeftChild() const {
	return children[0].get();
}

template<class OBJ> std::shared_ptr<AABBNode<OBJ>> AABBNode<OBJ>::getLeftChildSmartPtr() const {
	return children[0];
}

template<class OBJ> void AABBNode<OBJ>::setRightChild(std::shared_ptr<AABBNode<OBJ>> rightChild) {
	children[1] = rightChild;
	if (rightChild) {
		children[1]->setParent(this);
	}
}

template<class OBJ> AABBNode<OBJ>* AABBNode<OBJ>::getRightChild() const {
	return children[1].get();
}

template<class OBJ> std::shared_ptr<AABBNode<OBJ>> AABBNode<OBJ>::getRightChildSmartPtr() const {
	return children[1];
}

template<class OBJ> std::shared_ptr<AABBNode<OBJ>> AABBNode<OBJ>::getSibling() const {
	return parentNode->getLeftChild() == this ? parentNode->getRightChildSmartPtr() : parentNode->getLeftChildSmartPtr();
}

/**
* Returns fat AABBox for leaf and bounding box for branch
*/
template<class OBJ> const AABBox<float>& AABBNode<OBJ>::getAABBox() const {
	return aabbox;
}

template<class OBJ> void AABBNode<OBJ>::updateAABBox(float fatMargin) {
	if (isLeaf()) {
		Point3 fatMargin3(fatMargin, fatMargin, fatMargin);
		AABBox<float> objectBox = getNodeData().retrieveObjectAABBox();

		aabbox = AABBox<float>(objectBox.getMin() - fatMargin3, objectBox.getMax() + fatMargin3);
	}
	else {
		aabbox = children[0]->getAABBox().merge(children[1]->getAABBox());
	}
}
