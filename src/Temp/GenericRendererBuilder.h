#pragma once

#include "DataContainer.h"
#include "IndexContainer.h"
#include "Common.h"
#include "BlendFunction.h"

class GenericRendererBuilder : public std::enable_shared_from_this<GenericRendererBuilder> {
public:
	static std::shared_ptr<GenericRendererBuilder> create(std::string, RenderTargetRef, ShaderProgramRef, PrimitiveTopology);

	const std::string& getName() const;
	RenderTargetRef getRenderTarget() const;
	ShaderProgramRef getShader() const;
	PrimitiveTopology getShapeType() const;

	std::shared_ptr<GenericRendererBuilder> addData(const std::vector<Point2<float>>&);
	std::shared_ptr<GenericRendererBuilder> addData(const std::vector<Point3<float>>&);
	std::shared_ptr<GenericRendererBuilder> addData(const std::vector<Vector3<float>>&);
	const std::vector<DataContainer>& getData() const;
	std::shared_ptr<GenericRendererBuilder> instanceData(std::size_t, VariableType, const float*);
	const std::shared_ptr<DataContainer>& getInstanceData() const;
	std::shared_ptr<GenericRendererBuilder> indices(const std::vector<uint32_t>&);
	const std::shared_ptr<IndexContainer>& getIndices() const;

	//std::shared_ptr<GenericRendererBuilder> addUniformData(std::size_t, const void*);
	//const std::vector<ShaderDataContainer>& getUniformData() const;

	//std::shared_ptr<GenericRendererBuilder> addUniformTextureReader(const std::shared_ptr<TextureReader>&);
	//std::shared_ptr<GenericRendererBuilder> addUniformTextureReaderArray(const std::vector<std::shared_ptr<TextureReader>>&);
	//const std::vector<std::vector<std::shared_ptr<TextureReader>>>& getUniformTextureReaders() const;

	std::shared_ptr<GenericRendererBuilder> enableTransparency(const std::vector<BlendFunction>&);
	const std::vector<BlendFunction>& getBlendFunctions() const;

	void setScissor(const Vector2<int>&, const Vector2<int>&);
	bool hasCustomScissor() const;
	const Vector2<int>& getScissorOffset() const;
	const Vector2<int>& getScissorSize() const;

	std::shared_ptr<GenericRendererBuilder> enableDepthTest();
	bool isDepthTestEnabled() const;
	std::shared_ptr<GenericRendererBuilder> enableDepthWrite();
	bool isDepthWriteEnabled() const;

	std::shared_ptr<GenericRendererBuilder> disableCullFace();
	bool isCullFaceEnabled() const;

	std::shared_ptr<GenericRendererBuilder> polygonMode(RasterizerFillMode);
	RasterizerFillMode getPolygonMode() const;

	//void build();

private:
	GenericRendererBuilder(std::string, RenderTargetRef, ShaderProgramRef, PrimitiveTopology);

	std::string name;
	RenderTargetRef renderTarget;
	ShaderProgramRef shader;
	PrimitiveTopology shapeType;

	std::vector<DataContainer> data;
	std::shared_ptr<DataContainer> mInstanceData;
	std::shared_ptr<IndexContainer> mIndices;
	//std::vector<ShaderDataContainer> uniformData;
	//std::vector<std::vector<std::shared_ptr<TextureReader>>> uniformTextureReaders;
	std::vector<BlendFunction> blendFunctions;
	bool customScissor;
	Vector2<int> scissorOffset;
	Vector2<int> scissorSize;
	bool depthTestEnabled;
	bool depthWriteEnabled;
	bool cullFaceEnabled;
	RasterizerFillMode pPolygonMode;
};