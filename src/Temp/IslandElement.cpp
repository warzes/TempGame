#include "stdafx.h"
#include "IslandElement.h"


IslandElement::IslandElement() :
	islandElementId(0) {

}

void IslandElement::setIslandElementId(unsigned int islandElementId) {
	this->islandElementId = islandElementId;
}

unsigned int IslandElement::getIslandElementId() const {
	return islandElementId;
}