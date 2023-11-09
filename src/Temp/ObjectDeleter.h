#pragma once

class CollisionConvexObject3D;

class ObjectDeleter {
public:
	void operator()(CollisionConvexObject3D*) const;
};