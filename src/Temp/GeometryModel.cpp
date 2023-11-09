#include "stdafx.h"
#include "GeometryModel.h"

namespace
{
	ShaderProgramRef GeometryModelShader = nullptr;
	Uniform GeometryModelShaderUniformProjectionMatrix;
	Uniform GeometryModelShaderUniformColor;

	[[nodiscard]] inline ShaderProgramRef createShader()
	{
		const char* vertexShaderText = R"(
layout(location = 0) in vec3 vertexPosition;
uniform mat4 projectionView;

void main()
{
	gl_Position = projectionView * vec4(vertexPosition, 1.0);
}
)";

		const char* fragmentShaderText = R"(
out vec4 FragColor;

uniform vec3 color;

void main()
{
	FragColor = vec4(color, 1.0);
}
)";

		return GetRenderSystem().CreateShaderProgram({ vertexShaderText }, { fragmentShaderText });
	}
}

GeometryModel::GeometryModel() :
	isInitialized(false),
	renderTarget(nullptr),
	color(Vector3<float>(0.0f, 1.0f, 0.0f)),
	polygonMode(RasterizerFillMode::Wireframe),
	alwaysVisible(false),
	cullFaceDisabled(false) {
}

void GeometryModel::initialize(RenderTargetRef renderTarget) 
{
	this->renderTarget = renderTarget;
	refreshRenderer();
	isInitialized = true;
}

void GeometryModel::refreshRenderer() 
{
	if (!renderTarget) {
		return;
	}

	if (!GeometryModelShader)
	{
		auto& render = GetRenderSystem();
		GeometryModelShader = createShader();
		GeometryModelShaderUniformProjectionMatrix = render.GetUniform(GeometryModelShader, "projectionView");
		GeometryModelShaderUniformColor = render.GetUniform(GeometryModelShader, "color");
	}


	std::vector<uint32_t> indices;
	std::vector<Point3<float>> vertexArray = retrieveVertexArray(indices);
	Matrix4<float> projectionViewModelMatrix;
	renderer = GenericRendererBuilder::create("geometry model", renderTarget, GeometryModelShader, getShapeType())
		->addData(vertexArray)
		//->addUniformData(sizeof(projectionViewModelMatrix), &projectionViewModelMatrix) //binding 0
		//->addUniformData(sizeof(color), &color) //binding 1
		->polygonMode(polygonMode);

	if (!indices.empty()) {
		renderer->indices(indices);
	}

	if (polygonMode == RasterizerFillMode::Wireframe) {
		renderer->disableCullFace();
	}

	if (!alwaysVisible) {
		renderer->enableDepthTest();
		renderer->enableDepthWrite();
	}

	if (cullFaceDisabled) {
		renderer->disableCullFace();
	}

	//renderer = rendererBuilder->build();
	//renderer->disableRenderer(); //in case this method is called after 'renderTarget->disableAllRenderers()'
}

RenderTargetRef GeometryModel::getRenderTarget() const 
{
	return renderTarget;
}

Vector3<float> GeometryModel::getColor() const {
	return color;
}

void GeometryModel::setColor(float red, float green, float blue) 
{
	GetRenderSystem().SetUniform(GeometryModelShaderUniformColor, { red, green, blue });
}

RasterizerFillMode GeometryModel::getPolygonMode() const
{
	return polygonMode;
}

void GeometryModel::setPolygonMode(RasterizerFillMode polygonMode) 
{
	this->polygonMode = polygonMode;
	refreshRenderer();
}

bool GeometryModel::isAlwaysVisible() const 
{
	return alwaysVisible;
}

void GeometryModel::setAlwaysVisible(bool alwaysVisible)
{
	this->alwaysVisible = alwaysVisible;
	refreshRenderer();
}

bool GeometryModel::isCullFaceDisabled() const {
	return cullFaceDisabled;
}

void GeometryModel::disableCullFace() {
	this->cullFaceDisabled = true;
	refreshRenderer();
}

void GeometryModel::setModelMatrix(const Matrix4<float>& modelMatrix) {
	this->modelMatrix = modelMatrix;
}

void GeometryModel::prepareRendering(unsigned int renderingOrder, const Matrix4<float>& projectionViewMatrix) const {
	if (!isInitialized) {
		throw std::runtime_error("Geometry model must be initialized before call prepare rendering");
	}

	Matrix4<float> projectionViewModelMatrix = projectionViewMatrix * modelMatrix;
	glm::mat4 mat = glm::mat4( // TODO: delete
		projectionViewModelMatrix.a11, projectionViewModelMatrix.a21, projectionViewModelMatrix.a31, projectionViewModelMatrix.a41,
		projectionViewModelMatrix.a12, projectionViewModelMatrix.a22, projectionViewModelMatrix.a32, projectionViewModelMatrix.a42,
		projectionViewModelMatrix.a13, projectionViewModelMatrix.a23, projectionViewModelMatrix.a33, projectionViewModelMatrix.a43,
		projectionViewModelMatrix.a14, projectionViewModelMatrix.a24, projectionViewModelMatrix.a34, projectionViewModelMatrix.a44);

	GetRenderSystem().SetUniform(GeometryModelShaderUniformProjectionMatrix, mat);
	// TODO: set render???
	//renderer->updateUniformData(0, &projectionViewModelMatrix);
	//renderer->enableRenderer(renderingOrder);

}