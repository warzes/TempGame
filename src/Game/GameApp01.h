#pragma once

class GameApp01 final : public IApp
{
	bool Create() final;
	void Destroy() final;

	void Render() final;
	void Update(float deltaTime) final;

private:
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	glm::mat4 m_perspective;
	Texture2DRef m_textures[2];

	Camera m_camera;
};