#pragma once

#include "DisplayMode.h"
#include "MeshFilter.h"
#include "CustomModelShaderVariable.h"
#include "AABBoxModel.h"

class ModelSetDisplayer;

class ModelInstanceDisplayer {
public:
	ModelInstanceDisplayer(const ModelSetDisplayer&, DisplayMode, RenderTargetRef, ShaderProgramRef);
	~ModelInstanceDisplayer();

	void setupCustomShaderVariable(CustomModelShaderVariable*);
	void setupDepthOperations(bool, bool);
	void setupFaceCull(bool);
	void setupBlendFunctions(const std::vector<BlendFunction>&);

	void initialize();

	void updateMeshVertices(const nModel*) const;
	void updateMeshUv(const nModel*) const;
	void updateMaterial(const nModel*) const;
	void updateScale() const;

	const ModelSetDisplayer& getModelSetDisplayer() const;
	std::size_t getInstanceId() const;
	void updateInstanceId(std::size_t);
	std::span<nModel* const> getInstanceModels() const;

	void addInstanceModel(nModel&);
	void removeInstanceModel(nModel&);
	unsigned int getInstanceCount() const;

	void resetRenderingModels();
	void registerRenderingModel(const nModel&);
	void prepareRendering(unsigned int, const Matrix4<float>&, const MeshFilter*) const;

	void drawBBox(GeometryContainer&);
	void drawBaseBones(GeometryContainer&, const MeshFilter*) const;

private:
	nModel& getReferenceModel() const;

	void fillMaterialData(const nMesh&) const;
	std::vector<Point2<float>> scaleUv(const std::vector<Point2<float>>&, const std::vector<Vector3<float>>&, const UvScale&) const;
	//TextureParam buildTextureParam(const Mesh&) const;

	bool isInitialized;

	const ModelSetDisplayer& modelSetDisplayer;
	std::vector<nModel*> instanceModels;
	std::size_t instanceId;
	DisplayMode displayMode;
	RenderTargetRef renderTarget;
	ShaderProgramRef shader;

	mutable Matrix4<float> projectionViewMatrix;
	struct InstanceMatrix {
		Matrix4<float> modelMatrix;
		Matrix4<float> normalMatrix;
	};
	mutable std::vector<InstanceMatrix> instanceMatrices; //for DEFAULT_MODE only
	mutable std::vector<Matrix4<float>> instanceModelMatrices; //for DEPTH_ONLY_MODE only
	mutable struct {
		alignas(4) float encodedEmissiveFactor;
		alignas(4) float ambientFactor;
	} materialData;

	CustomModelShaderVariable* customShaderVariable;
	bool depthTestEnabled;
	bool depthWriteEnabled;
	bool enableFaceCull;
	std::vector<BlendFunction> blendFunctions;

	//std::vector<std::unique_ptr<GenericRenderer>> meshRenderers;
	std::vector<std::shared_ptr<AABBoxModel>> aabboxModels;
};