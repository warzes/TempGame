#pragma once

#include "CollisionShape3D.h"
#include "GhostBody.h"

class PhysicsCharacter {
public:
	PhysicsCharacter(std::string, float, std::unique_ptr<const CollisionShape3D>, const PhysicsTransform&);

	const std::string& getName() const;

	float getMass() const;

	const CollisionShape3D& getShape() const;

	void updateTransform(const PhysicsTransform&);
	const PhysicsTransform& getTransform() const;

private:
	std::string name;
	float mass;
	std::unique_ptr<const CollisionShape3D> shape;
	PhysicsTransform transform;
};