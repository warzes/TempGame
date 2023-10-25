#include "stdafx.h"
#include "EagerPropertyLoader.h"

float EagerPropertyLoader::getCollisionShapeInnerMargin() 
{
	//static float collisionShapeInnerMargin = ConfigService::instance().getFloatValue("collisionShape.innerMargin");
	static float collisionShapeInnerMargin = 0.04;


	return collisionShapeInnerMargin;
}