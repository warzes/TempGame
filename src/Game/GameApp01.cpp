#include "stdafx.h"
#include "GameApp01.h"
#include "BaseSceneShader.h"
#include "Vertex.h"
#include "MapEngine.h"
//-----------------------------------------------------------------------------
namespace
{
	ShaderProgramRef BaseSceneShader;
	Uniform BaseSceneShaderUniformProjectionMatrix;
	Map map;
}
//-----------------------------------------------------------------------------
bool GameApp01::Create()
{
	//glEnable(GL_CULL_FACE); // для теста - треугольник выше против часой стрелки

	BaseVertex vert[] =
	{
		// вверх
		BaseVertex({ -0.5f, 0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({  0.5f, 0.5f,  0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f, 0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({  0.5f, 0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),

		// лево
		BaseVertex({ -0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),

		// право
		BaseVertex({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),

		// низ
		BaseVertex({  0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({  0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),

		// перед
		BaseVertex({ -0.5f,  0.5f, -0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({  0.5f,  0.5f, -0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({  0.5f, -0.5f, -0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),

		// зад
		BaseVertex({  0.5f,  0.5f, 0.5f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f,  0.5f, 0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({  0.5f, -0.5f, 0.5f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),
		BaseVertex({ -0.5f, -0.5f, 0.5f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}),

	};


	unsigned int indices[] = {
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23
	};

	auto& renderSystem = GetRenderSystem();
	auto& graphicsSystem = GetGraphicsSystem();

	BaseSceneShader = CreateBaseSceneShader();
	BaseSceneShaderUniformProjectionMatrix = renderSystem.GetUniform(BaseSceneShader, "pvMatrix");

	m_geom = renderSystem.CreateGeometryBuffer(BufferUsage::StaticDraw, (unsigned)Countof(vert), (unsigned)sizeof(BaseVertex), vert, (unsigned)Countof(indices), IndexFormat::UInt32, indices, BaseSceneShader);

	m_texture = renderSystem.CreateTexture2D("../Data/Textures/texel_checker.png");

	m_camera.Teleport(glm::vec3(0.0f, 1.0f, -3.0f));

	GetInputSystem().SetMouseLock(true);


	for (size_t x = 0; x < AreaSizeXZ; x++)
	{
		for (size_t z = 0; z < AreaSizeXZ; z++)
		{
			map.SetTile(
				{ 
					.desc = { .type = TileType::Solid },
				}, x, z);
		}
	}

	//map.SetTile(
	//	{
	//		.desc = {.type = TileType::None },
	//	}, 5, 5);

	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = 4,
		}, 5, 5);
	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = -4,
		}, 6, 5);
	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = 2,
		}, 7, 5);
	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = 1,
		}, 8, 5);

	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = -17,
		}, 4, 5);

	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = -4,
		}, 5, 4);

	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = -4,
		}, 5, 6);


	map.SetTile(
		{
			.desc = {.type = TileType::Solid },
			.posY = -4,
		}, 4, 4);

	map.BuildMesh(BaseSceneShader);






	return true;
}
//-----------------------------------------------------------------------------
void GameApp01::Destroy()
{
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

	renderSystem.Bind(m_texture);
	renderSystem.Bind(BaseSceneShader);
	renderSystem.SetUniform(BaseSceneShaderUniformProjectionMatrix, m_perspective * m_camera.GetViewMatrix());
	renderSystem.SetUniform("Texture", 0);
	renderSystem.Draw(map.geom);
	//renderSystem.Draw(m_geom);

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