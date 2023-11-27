#include "stdafx.h"
#include "GameApp.h"
#include "BaseSceneShader.h"
#include "World.h"
//-----------------------------------------------------------------------------
namespace
{
	World world;



	ShaderProgramRef NewMeshShader;
	Uniform NewMeshShaderUniformProjectionMatrix;
	Uniform NewMeshShaderUniformViewMatrix;
	Uniform NewMeshShaderWorldViewMatrix;

	NewModel tempModel4;

	Texture2DRef TestTexture;
}
//-----------------------------------------------------------------------------
bool GameApp::Create()
{
	if (!world.Create())
		return false;



	auto& renderSystem = GetRenderSystem();

	NewMeshShader = CreateNewMeshSceneShader();
	NewMeshShaderUniformProjectionMatrix = renderSystem.GetUniform(NewMeshShader, "uProjection");
	NewMeshShaderUniformViewMatrix = renderSystem.GetUniform(NewMeshShader, "uView");
	NewMeshShaderWorldViewMatrix = renderSystem.GetUniform(NewMeshShader, "uWorld");

	tempModel4 = LoadModel("../Data/Models/castle.obj");

	m_textures[0] = renderSystem.CreateTexture2D("../Data/Textures/testTop.png");

	Texture2DInfo info;
	info.wrapS = TextureAddressMode::ClampToEdge;
	info.wrapT = TextureAddressMode::ClampToEdge;
	m_textures[1] = renderSystem.CreateTexture2D("../Data/Textures/testWall.png", true, info);

	m_camera.Teleport(glm::vec3(0.0f, 1.0f, -3.0f));

	GetInputSystem().SetMouseLock(true);
		
	TestTexture = renderSystem.CreateTexture2D("../Data/Textures/testWall.png");

	return true;
}
//-----------------------------------------------------------------------------
void GameApp::Destroy()
{
	world.Destroy();
	m_textures[0].reset();
	m_textures[1].reset();
}
//-----------------------------------------------------------------------------
void GameApp::Render()
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
	renderSystem.ClearFrame();
		
	renderSystem.Bind(NewMeshShader);
	renderSystem.Bind(TestTexture);
	renderSystem.SetUniform(NewMeshShaderUniformProjectionMatrix, m_perspective);
	renderSystem.SetUniform(NewMeshShaderUniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(NewMeshShaderWorldViewMatrix, glm::mat4(1.0f));
	renderSystem.SetUniform("Texture", 0);

	renderSystem.SetUniform(NewMeshShaderWorldViewMatrix, glm::translate(glm::mat4(1.0f), glm::vec3(-9.0f, 0.0f, 40.0f)));
	for (size_t i = 0; i < tempModel4.meshes.size(); i++)
	{
		renderSystem.Draw(tempModel4.meshes[i].geometry);
	}

	world.Draw();
}
//-----------------------------------------------------------------------------
void GameApp::Update(float deltaTime)
{
	if (GetInputSystem().IsKeyDown(Input::KEY_ESCAPE))
	{
		ExitRequest();
		return;
	}

	const float mouseSensitivity = 10.0f * deltaTime;
	const float moveSpeed = 20.0f * deltaTime;

	if (GetInputSystem().IsKeyDown(Input::KEY_W)) m_camera.MoveBy(moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_S)) m_camera.MoveBy(-moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_A)) m_camera.StrafeBy(moveSpeed);
	if (GetInputSystem().IsKeyDown(Input::KEY_D)) m_camera.StrafeBy(-moveSpeed);

	glm::vec2 delta = GetInputSystem().GetMouseDeltaPosition();
	if (delta.x != 0.0f)  m_camera.RotateLeftRight(delta.x * mouseSensitivity);
	if (delta.y != 0.0f)  m_camera.RotateUpDown(-delta.y * mouseSensitivity);

	world.Update(deltaTime);
}
//-----------------------------------------------------------------------------