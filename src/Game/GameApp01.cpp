#include "stdafx.h"
#include "GameApp01.h"
#include "BaseSceneShader.h"
#include "Vertex.h"
//-----------------------------------------------------------------------------
namespace
{
	ShaderProgramRef BaseSceneShader;
	Uniform BaseSceneShaderUniformProjectionMatrix;
	ShaderProgramRef MeshSceneShader;
	Uniform MeshSceneShaderUniformWorldMatrix;
	Uniform MeshSceneShaderUniformViewMatrix;
	Uniform MeshSceneShaderUniformProjectionMatrix;

	RenderTargetRef rt;
}
//-----------------------------------------------------------------------------
bool GameApp01::Create()
{
	BaseVertex vert[] =
	{
		BaseVertex({ 1000.f * -0.5f, 0.0f, 1000.0f * 0.5f}, {0.0f, 0.0f},       {1.0f, 1.0f, 1.0f, 1.0f}), // top left
		BaseVertex({ 1000.f * 0.5f, 0.0f, 1000.0f * 0.5f}, {1000.0f, 0.0f},    {1.0f, 1.0f, 1.0f, 1.0f}), // top right
		BaseVertex({ 1000.f * -0.5f, 0.0f, 1000.0f * -0.5f}, {0.0f, 1000.0f},    {1.0f, 1.0f, 1.0f, 1.0f}), // bottom right
		BaseVertex({ 1000.f * 0.5f, 0.0f, 1000.0f * -0.5f}, {1000.0f, 1000.0f}, {1.0f, 1.0f, 1.0f, 1.0f}), // bottom left
	};
	//glEnable(GL_CULL_FACE); // для теста - треугольник выше против часой стрелки

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,   // first triangle
		2, 1, 3    // second triangle
	};

	auto& renderSystem = GetRenderSystem();
	auto& graphicsSystem = GetGraphicsSystem();

	rt = graphicsSystem.CreateRenderTarget(1024, 768);

	BaseSceneShader = CreateBaseSceneShader();
	BaseSceneShaderUniformProjectionMatrix = renderSystem.GetUniform(BaseSceneShader, "pvMatrix");

	MeshSceneShader = CreateMeshSceneShader();
	MeshSceneShaderUniformWorldMatrix = renderSystem.GetUniform(MeshSceneShader, "uWorld");
	MeshSceneShaderUniformViewMatrix = renderSystem.GetUniform(MeshSceneShader, "uView");
	MeshSceneShaderUniformProjectionMatrix = renderSystem.GetUniform(MeshSceneShader, "uProjection");

	m_geom = renderSystem.CreateGeometryBuffer(BufferUsage::StaticDraw, (unsigned)Countof(vert), (unsigned)sizeof(BaseVertex), vert,
		(unsigned)Countof(indices), IndexFormat::UInt32, indices, BaseSceneShader);

	m_texture = renderSystem.CreateTexture2D("../Data/Textures/texel_checker.png");

	m_model = graphicsSystem.CreateModel("../Data/Models/crate.obj", "../Data/Models/");
	m_model->subMeshes[0].material.diffuseTexture = m_texture;

	m_camera.Teleport(glm::vec3(0.0f, 5.0f, -5.0f));

	GetInputSystem().SetMouseLock(true);

	return true;
}
//-----------------------------------------------------------------------------
void GameApp01::Destroy()
{
	m_model.reset();
	MeshSceneShader.reset();
	BaseSceneShader.reset();
	m_geom.reset();
	m_texture.reset();
}
//-----------------------------------------------------------------------------
void GameApp01::Render()
{
	auto& renderSystem = GetRenderSystem();
	auto& graphicsSystem = GetGraphicsSystem();

	if (m_windowWidth != GetWindowWidth() || m_windowHeight != GetWindowHeight())
	{
		m_windowWidth = GetWindowWidth();
		m_windowHeight = GetWindowHeight();
		m_perspective = glm::perspective(glm::radians(65.0f), GetWindowSizeAspect(), 0.01f, 100.f);
		renderSystem.SetViewport(m_windowWidth, m_windowHeight);
	}
	
	// не рисует с NOT_USE_LEFT_HANDED_MATH

	renderSystem.ClearFrame();

	{
		graphicsSystem.BindRenderTarget(rt);
		renderSystem.SetClearColor({ 0.1f, 0.1f, 0.1f });
		renderSystem.ClearFrame();

		renderSystem.Bind(m_texture);
		renderSystem.Bind(BaseSceneShader);
		renderSystem.SetUniform(BaseSceneShaderUniformProjectionMatrix, m_perspective * m_camera.GetViewMatrix());
		renderSystem.SetUniform("Texture", 0);
		renderSystem.Draw(m_geom->vao);


		renderSystem.Bind(m_texture);
		renderSystem.Bind(MeshSceneShader);
		renderSystem.SetUniform("DiffuseTexture", 0);
		renderSystem.SetUniform(MeshSceneShaderUniformWorldMatrix, glm::mat4(1.0f));
		renderSystem.SetUniform(MeshSceneShaderUniformViewMatrix, m_camera.GetViewMatrix());
		renderSystem.SetUniform(MeshSceneShaderUniformProjectionMatrix, m_perspective);
		graphicsSystem.Draw(m_model);
	}

	renderSystem.MainScreen();
	renderSystem.SetClearColor({ 0.1f, 0.1f, 0.1f });

	graphicsSystem.BindRenderTargetAsTexture(rt, 0);
	renderSystem.Bind(BaseSceneShader);
	renderSystem.SetUniform(BaseSceneShaderUniformProjectionMatrix, m_perspective * m_camera.GetViewMatrix());
	renderSystem.SetUniform("Texture", 0);
	renderSystem.Draw(m_geom->vao);

	renderSystem.Bind(m_texture);
	renderSystem.Bind(MeshSceneShader);
	renderSystem.SetUniform("DiffuseTexture", 0);
	renderSystem.SetUniform(MeshSceneShaderUniformWorldMatrix, glm::mat4(1.0f));
	renderSystem.SetUniform(MeshSceneShaderUniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(MeshSceneShaderUniformProjectionMatrix, m_perspective);
	graphicsSystem.Draw(m_model);
}
//-----------------------------------------------------------------------------
void GameApp01::Update(float deltaTime)
{
	if (GetInputSystem().IsKeyDown(Input::KEY_ESCAPE))
	{
		ExitRequest();
		return;
	}

	const float mouseSensitivity = 10.0f * deltaTime;
	const float moveSpeed = 10.0f * deltaTime;

	if (GetInputSystem().IsKeyDown(Input::KEY_W)) m_camera.MoveBy(moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_S)) m_camera.MoveBy(-moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_A)) m_camera.StrafeBy(moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_D)) m_camera.StrafeBy(-moveSpeed);

	glm::vec2 delta = GetInputSystem().GetMouseDeltaPosition();
	if (delta.x != 0.0f)  m_camera.RotateLeftRight(delta.x * mouseSensitivity);
	if (delta.y != 0.0f)  m_camera.RotateUpDown(-delta.y * mouseSensitivity);
}
//-----------------------------------------------------------------------------