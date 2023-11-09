#pragma once

#include "Camera.h"

/**
* A FPS camera:
* - mouse allow to rotate camera based on mouse movements
* - directional keys allow to move camera forward/backward/left/right
*/
class FpsCamera : public nCamera {
public:
	FpsCamera(float, float, float);
	~FpsCamera() override = default;

	void setSpeed(float);

	void moveForward(float) override;
	void moveBackward(float) override;
	void moveLeft(float) override;
	void moveRight(float) override;

private:
	float speed;
};