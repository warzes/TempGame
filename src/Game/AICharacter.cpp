#include "stdafx.h"
#include "AICharacter.h"

AICharacter::AICharacter(float mass, float maxVelocityInKmH, const Point3<float>& position) :
	mass(mass),
	maxVelocityInKmH(maxVelocityInKmH),
	velocity(Vector3<float>(0.0f, 0.0f, 0.0f)),
	position(position) {

}

float AICharacter::getMass() const {
	return mass;
}

float AICharacter::getMaxVelocityInKmH() const {
	return maxVelocityInKmH;
}

float AICharacter::retrieveMaxVelocityInMs() const {
	return (maxVelocityInKmH * 1000.0f) / (60.0f * 60.0f);
}

void AICharacter::updateVelocity(const Vector3<float>& velocity) {
	this->velocity = velocity;
}

const Vector3<float>& AICharacter::getVelocity() const {
	return velocity;
}

void AICharacter::updatePosition(const Point3<float>& position) {
	this->position = position;
}

const Point3<float>& AICharacter::getPosition() const {
	return position;
}