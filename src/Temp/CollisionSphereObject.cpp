#include "stdafx.h"
#include "CollisionSphereObject.h"

CollisionSphereObject::CollisionSphereObject(float radius, const Point3<float>& centerOfMass) :
	CollisionConvexObject3D(radius),
	sphereObject(nSphere<float>(radius, centerOfMass)) {

}

float CollisionSphereObject::getRadius() const {
	return getOuterMargin();
}

const Point3<float>& CollisionSphereObject::getCenterOfMass() const {
	return sphereObject.getCenterOfMass();
}

CollisionConvexObject3D::ObjectType CollisionSphereObject::getObjectType() const {
	return CollisionConvexObject3D::SPHERE_OBJECT;
}

/**
* @return includeMargin Indicate whether support function need to take into account margin
*/
Point3<float> CollisionSphereObject::getSupportPoint(const Vector3<float>& direction, bool includeMargin) const {
	if (includeMargin) {
		return retrieveSphere().getSupportPoint(direction);
	}

	return sphereObject.getCenterOfMass();
}

nSphere<float> CollisionSphereObject::retrieveSphere() const {
	return nSphere<float>(getRadius(), getCenterOfMass());
}

std::string CollisionSphereObject::toString() const {
	std::stringstream ss;
	ss.precision(std::numeric_limits<float>::max_digits10);

	ss << "Collision sphere:" << std::endl;
	ss << std::setw(20) << std::left << " - Outer margin: " << getOuterMargin() << std::endl;
	ss << std::setw(20) << std::left << " - Radius: " << getRadius() << std::endl;
	ss << std::setw(20) << std::left << " - Center of mass: " << getCenterOfMass();

	return ss.str();
}