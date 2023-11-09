#pragma once

class AICharacterEventHandler {
public:
	virtual ~AICharacterEventHandler() = default;

	virtual void startMoving() {}
	virtual void stopMoving() {}
};