#include "stdafx.h"
#include "GameApp01.h"
#include "BaseSceneShader.h"
#include "Vertex.h"
#include "MapEngine.h"

/*

блокам сделать возможность смещаться на 1-2 пикселя вверх или вниз чтобы полчить эффект
https://www.youtube.com/watch?v=OeY_0Mk_0DQ

подумать над этим
	https://www.youtube.com/watch?v=FuVlAJl5NOc

*/
//
//по юнити и данженкравлу
//вот в таком же стиле
//https ://github.com/davemoore22/sorcery
//
//
//по кубу
//https ://nusan.itch.io/pcraft
//https://github.com/jdah/microcraft
//
//возможность убирать стены(например в будущем можно добавлять тайл деталиции - тогда не надо рисовать стену за ним
//
//
//	по тайлам - чтобы не сливалось - можно края рисовать более темным(например края травы темнозеленым.как на 9d0cf182aa9af61f8c4b3675c9cfdeea.jpg
//
//
//
//
//		новый метод блоков
//
//		Из вокселей
//
//		при этом называется 3д тайлом
//
//		есть файл описания при создании
//
//		при этом нужно резать модели по стороном(лево, право, вверх, вниз, перед, зад) - чтобы можно было эффективно выкидывать невидимое
//

//-----------------------------------------------------------------------------
namespace
{
	ShaderProgramRef TileShader;
	Uniform TileShaderUniformProjectionMatrix;
	Uniform TileShaderUniformViewMatrix;
	Uniform TileShaderWorldViewMatrix;

	ShaderProgramRef NewMeshShader;
	Uniform NewMeshShaderUniformProjectionMatrix;
	Uniform NewMeshShaderUniformViewMatrix;
	Uniform NewMeshShaderWorldViewMatrix;
	Map map;

	NewModel tempModel;
	// Load gltf model animations
	unsigned int animsCount = 0;
	unsigned int animIndex = 0;
	unsigned int animCurrentFrame = 0;
	ModelAnimation* modelAnimations;

	Texture2DRef TestTexture;
	ShaderProgramRef TestShader;
	Uniform TestUniformProjectionMatrix;
	GeometryBufferRef TestGeom;
}
//-----------------------------------------------------------------------------
bool GameApp01::Create()
{
	//glEnable(GL_CULL_FACE); // для теста - треугольник выше против часой стрелки

	auto& renderSystem = GetRenderSystem();

	NewMeshShader = CreateNewMeshSceneShader();
	NewMeshShaderUniformProjectionMatrix = renderSystem.GetUniform(NewMeshShader, "uProjection");
	NewMeshShaderUniformViewMatrix = renderSystem.GetUniform(NewMeshShader, "uView");
	NewMeshShaderWorldViewMatrix = renderSystem.GetUniform(NewMeshShader, "uWorld");

	//tempModel = LoadModel("../Data/Models/robot.glb");
	//modelAnimations = LoadModelAnimations("../Data/Models/robot.glb", animsCount);
	//tempModel = LoadModel("../Data/Models/cesium_man.m3d");
	//modelAnimations = LoadModelAnimations("../Data/Models/cesium_man.m3d", animsCount);
	tempModel = LoadModel("../Data/Models/guy.iqm");
	modelAnimations = LoadModelAnimations("../Data/Models/guyanim.iqm", animsCount);





	TileShader = CreateTileSceneShader();
	TileShaderUniformProjectionMatrix = renderSystem.GetUniform(TileShader, "projectionMatrix");
	TileShaderUniformViewMatrix = renderSystem.GetUniform(TileShader, "viewMatrix");
	TileShaderWorldViewMatrix = renderSystem.GetUniform(TileShader, "worldMatrix");

	m_textures[0] = renderSystem.CreateTexture2D("../Data/Textures/testTop.png");

	Texture2DInfo info;
	info.wrapS = TextureAddressMode::ClampToEdge;
	info.wrapT = TextureAddressMode::ClampToEdge;
	m_textures[1] = renderSystem.CreateTexture2D("../Data/Textures/testWall.png", true, info);

	m_camera.Teleport(glm::vec3(0.0f, 1.0f, -3.0f));

	GetInputSystem().SetMouseLock(true);

	{
		for (size_t x = 0; x < AreaSizeXZ; x++)
		{
			for (size_t z = 0; z < AreaSizeXZ; z++)
			{
				map.SetTile(
					{
						.type = TileType::Solid,
						.textureWallId = 1
					}, x, z);
			}
		}

		//map.SetTile(
		//	{
		//		.desc = {.type = TileType::None },
		//	}, 5, 5);

		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = 4,
			}, 5, 5);
		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = -4,
			}, 6, 5);
		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = 2,
			}, 7, 5);
		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = 1,
			}, 8, 5);

		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = -17,
			}, 4, 5);

		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = -4,
			}, 5, 4);

		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = -4,
			}, 5, 6);


		map.SetTile(
			{
				.type = TileType::Solid,
				.textureWallId = 1,
				.posY = -4,
			}, 4, 4);


		//map.SetTile(
		//	{
		//		.type = TileType::Solid,
		//		.textureWallId = 1,
		//		.posY = 4,
		//	}, 7, 2);

		map.SetTile(
			{
				.type = TileType::RampL,
				.textureWallId = 1,
				.posY = 4,
			}, 6, 2);

		map.SetTile(
			{
				.type = TileType::RampR,
				.textureWallId = 1,
				.posY = 4,
			}, 8, 2);


		map.SetTile(
			{
				.type = TileType::RampT,
				.textureWallId = 1,
				.posY = 4,
			}, 7, 1);

		map.SetTile(
			{
				.type = TileType::RampB,
				.textureWallId = 1,
				.posY = 4,
			}, 7, 3);



		map.BuildMesh(TileShader);
	}

	



	struct testVertex
	{
		glm::vec3 pos;
		glm::vec2 texCoord;
	}
	vert[] =
	{
		{{ -0.5f,  0.5f, 2.0f}, {0.0f, 0.0f}}, // top left
		{{  0.5f,  0.5f, 2.0f}, {1.0f, 0.0f}}, // top right
		{{  0.5f, -0.5f, 2.0f}, {1.0f, 1.0f}}, // bottom right
		{{ -0.5f, -0.5f, 2.0f}, {0.0f, 1.0f}}, // bottom left

	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 3, 2,   // first triangle
		2, 1, 0    // second triangle
	};

	TestShader = CreateTestShader();
	TestUniformProjectionMatrix = renderSystem.GetUniform(TestShader, "projectionMatrix");

	TestGeom = renderSystem.CreateGeometryBuffer(BufferUsage::StaticDraw, (unsigned)Countof(vert), (unsigned)sizeof(testVertex), vert, (unsigned)Countof(indices), IndexFormat::UInt32, indices, TestShader);

	TestTexture = renderSystem.CreateTexture2D("../Data/Textures/testWall.png");

	return true;
}
//-----------------------------------------------------------------------------
void GameApp01::Destroy()
{
	TileShader.reset();
	m_textures[0].reset();
	m_textures[1].reset();
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
	
	renderSystem.Bind(TileShader);
	renderSystem.SetUniform(TileShaderUniformProjectionMatrix, m_perspective);
	renderSystem.SetUniform(TileShaderUniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(TileShaderWorldViewMatrix, glm::mat4(1.0f));
	renderSystem.SetUniform("Texture", 0);

	for (size_t i = 0; i < map.geomMap.arr.size(); i++)
	{
		if (map.geomMap.arr[i].textureId == 0)
			renderSystem.Bind(m_textures[0]);
		else if (map.geomMap.arr[i].textureId == 1)
			renderSystem.Bind(m_textures[1]);

		renderSystem.Draw(map.geomMap.arr[i].geom);
	}

	renderSystem.Bind(NewMeshShader);
	renderSystem.SetUniform(NewMeshShaderUniformProjectionMatrix, m_perspective);
	renderSystem.SetUniform(NewMeshShaderUniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(NewMeshShaderWorldViewMatrix, glm::mat4(1.0f));
	renderSystem.SetUniform("Texture", 0);

	for (size_t i = 0; i < tempModel.meshes.size(); i++)
	{
		renderSystem.Draw(tempModel.meshes[i].geometry);
	}

	//renderSystem.Bind(TestTexture, 0);
	//renderSystem.Bind(TestShader);
	//renderSystem.SetUniform(TestUniformProjectionMatrix, m_perspective);
	//renderSystem.SetUniform("Texture", 0);
	//renderSystem.Draw(TestGeom);
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


	/*
	ANIM
	*/

	// Update model animation
	ModelAnimation anim = modelAnimations[animIndex];
	animCurrentFrame = (animCurrentFrame + 1) % anim.frameCount;
	UpdateModelAnimation(tempModel, anim, animCurrentFrame);
}
//-----------------------------------------------------------------------------