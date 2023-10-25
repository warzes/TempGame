#pragma once

#include "Common.h"
#include "GenericRendererBuilder.h"

class GeometryModel 
{
public:
	friend class GeometryContainer;
	GeometryModel();
	virtual ~GeometryModel() = default;

	RenderTargetRef getRenderTarget() const;

	Vector3<float> getColor() const;
	void setColor(float, float, float);

	RasterizerFillMode getPolygonMode() const;
	void setPolygonMode(RasterizerFillMode);

	bool isAlwaysVisible() const;
	void setAlwaysVisible(bool);

	bool isCullFaceDisabled() const;
	void disableCullFace();

	void setModelMatrix(const Matrix4<float>&);

protected:
	void initialize(RenderTargetRef);
	void refreshRenderer();

	virtual std::vector<Point3<float>> retrieveVertexArray(std::vector<uint32_t>&) const = 0;
	virtual PrimitiveTopology getShapeType() const = 0;

	void prepareRendering(unsigned int, const Matrix4<float>&) const;

private:
	bool isInitialized;

	RenderTargetRef renderTarget;
	std::shared_ptr<GenericRendererBuilder> renderer;
	Matrix4<float> modelMatrix;

	Vector3<float> color;
	RasterizerFillMode polygonMode;
	bool alwaysVisible;
	bool cullFaceDisabled;
};