#include "stdafx.h"
#include "TestApp.h"
#include "BaseSceneShader.h"
#include "Vertex.h"
#include "PhysicsEngine.h"
//-----------------------------------------------------------------------------
namespace
{
	ShaderProgramRef BaseSceneShader;
	Uniform BaseSceneShaderUniformProjectionMatrix;
	ShaderProgramRef MeshSceneShader;
	Uniform MeshSceneShaderUniformWorldMatrix;
	Uniform MeshSceneShaderUniformViewMatrix;
	Uniform MeshSceneShaderUniformProjectionMatrix;

	std::unique_ptr<PhysicsWorld> physicsWorld;
	CharacterControllerConfig characterControllerConfig;
	std::unique_ptr<CharacterController> characterController;
	std::shared_ptr<RigidBody> rb_ground;
	std::shared_ptr<RigidBody> rb_box;
	PhysicsTransform boxPos;

	RenderTargetRef rt;
}
//-----------------------------------------------------------------------------
bool TestApp::Create()
{
	BaseVertex vert[] =
	{
		BaseVertex({ 1000.f * -0.5f, 0.0f, 1000.0f *  0.5f}, {0.0f, 0.0f},       {1.0f, 1.0f, 1.0f, 1.0f}), // top left
		BaseVertex({ 1000.f *  0.5f, 0.0f, 1000.0f *  0.5f}, {1000.0f, 0.0f},    {1.0f, 1.0f, 1.0f, 1.0f}), // top right
		BaseVertex({ 1000.f * -0.5f, 0.0f, 1000.0f * -0.5f}, {0.0f, 1000.0f},    {1.0f, 1.0f, 1.0f, 1.0f}), // bottom right
		BaseVertex({ 1000.f *  0.5f, 0.0f, 1000.0f * -0.5f}, {1000.0f, 1000.0f}, {1.0f, 1.0f, 1.0f, 1.0f}), // bottom left
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

	physicsWorld = std::make_unique<PhysicsWorld>();

	auto groundShape = std::make_unique<CollisionBoxShape>(Vector3<float>(1000.0f, 0.5f, 1000.0f));
	rb_ground = std::make_shared<RigidBody>("ground", PhysicsTransform(Point3<float>(0.0f, -0.5f, 0.0f), Quaternion<float>()), std::move(groundShape));

	auto boxShape = std::make_unique<CollisionBoxShape>(Vector3<float>(0.5f, 0.5f, 0.5f));
	rb_box = std::make_shared<RigidBody>("box", PhysicsTransform(Point3<float>(0.0f, 20.5f, 0.0f), Quaternion<float>()), std::move(boxShape));
	rb_box->setMass(10.0f);

	physicsWorld->addBody(rb_ground);
	physicsWorld->addBody(rb_box);

	physicsWorld->setUp(1.0f / 60.0f);
	physicsWorld->unpause();

	GetInputSystem().SetMouseLock(true);

	return true;
}
//-----------------------------------------------------------------------------
void TestApp::Destroy()
{
	physicsWorld.reset();
	m_model.reset();
	MeshSceneShader.reset();
	BaseSceneShader.reset();
	m_geom.reset();
	m_texture.reset();
}
//-----------------------------------------------------------------------------
void TestApp::Render()
{
	auto& renderSystem = GetRenderSystem();
	auto& graphicsSystem = GetGraphicsSystem();

	if( m_windowWidth != GetWindowWidth() || m_windowHeight != GetWindowHeight() )
	{
		m_windowWidth = GetWindowWidth();
		m_windowHeight = GetWindowHeight();
		m_perspective = glm::perspective(glm::radians(65.0f), GetWindowSizeAspect(), 0.01f, 100.f);
		renderSystem.SetViewport(m_windowWidth, m_windowHeight);
	}

	auto matt = boxPos.toTransform().getTransformMatrix();
	glm::mat4 pos = glm::mat4(
		matt.a11, matt.a21, matt.a31, matt.a41,
		matt.a12, matt.a22, matt.a32, matt.a42,
		matt.a13, matt.a23, matt.a33, matt.a43,
		matt.a14, matt.a24, matt.a34, matt.a44);

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
		renderSystem.SetUniform(MeshSceneShaderUniformWorldMatrix, pos);
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
	renderSystem.SetUniform(MeshSceneShaderUniformWorldMatrix, pos);
	renderSystem.SetUniform(MeshSceneShaderUniformViewMatrix, m_camera.GetViewMatrix());
	renderSystem.SetUniform(MeshSceneShaderUniformProjectionMatrix, m_perspective);
	graphicsSystem.Draw(m_model);
}
//-----------------------------------------------------------------------------
void TestApp::Update(float deltaTime)
{
	if(GetInputSystem().IsKeyDown(Input::KEY_ESCAPE) )
	{
		ExitRequest();
		return;
	}

	for (size_t i = 0; i < physicsWorld->getBodyContainer().getBodies().size(); i++)
	{
		if (i == 1) 
		{
			auto bod = physicsWorld->getBodyContainer().getBodies()[i];
			boxPos = bod->getTransform();
		}
	}

	const float mouseSensitivity = 10.0f * deltaTime;
	const float moveSpeed = 10.0f * deltaTime;

	if(GetInputSystem().IsKeyDown(Input::KEY_W) ) m_camera.MoveBy(moveSpeed);
	if(GetInputSystem().IsKeyDown(Input::KEY_S) ) m_camera.MoveBy(-moveSpeed);
	if(GetInputSystem().IsKeyDown(Input::KEY_A) ) m_camera.StrafeBy(moveSpeed);
	if(GetInputSystem().IsKeyDown(Input::KEY_D) ) m_camera.StrafeBy(-moveSpeed);

	glm::vec2 delta = GetInputSystem().GetMouseDeltaPosition();
	if( delta.x != 0.0f )  m_camera.RotateLeftRight(delta.x * mouseSensitivity);
	if( delta.y != 0.0f )  m_camera.RotateUpDown(-delta.y * mouseSensitivity);
}
//-----------------------------------------------------------------------------