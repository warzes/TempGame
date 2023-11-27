#pragma once

class World
{
public:
	bool Create();
	void Destroy();

	void Update(float deltaTime);
	void Draw();
};