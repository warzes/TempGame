#pragma once

#include "Camera.h"

/**
* A free camera:
* - mouse right click released: directional keys allow to move camera forward/backward and rotate camera
* - mouse right click pressed: camera rotate based on mouse movements and directional keys allow to move camera forward/backward/left/right
*/
class FreeCamera : public nCamera {
public:
	FreeCamera(float, float, float);
	~FreeCamera() override = default;

	void setSpeed(float, float);

	bool onKeyPress() override;
	bool onKeyRelease() override;

	void moveForward(float) override;
	void moveBackward(float) override;
	void moveLeft(float) override;
	void moveRight(float) override;

private:
	float speed;
	float rotateSpeed;
};