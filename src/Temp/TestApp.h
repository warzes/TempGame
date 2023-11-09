#pragma once

class TestApp final : public IApp
{
	bool Create() final;
	void Destroy() final;

	void Render() final;
	void Update(float deltaTime) final;

private:
	int m_windowWidth = 0;
	int m_windowHeight = 0;

	glm::mat4 m_perspective;
	GeometryBufferRef m_geom;
	StaticModelRef m_model;
	Texture2DRef m_texture;

	Camera m_camera;
};