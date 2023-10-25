#pragma once

#include "GeometryModel.h"

class GeometryContainer {
public:
	explicit GeometryContainer(RenderTargetRef);

	void addGeometry(std::shared_ptr<GeometryModel>);
	void removeGeometry(const GeometryModel&);

	void prepareRendering(unsigned int&, const Matrix4<float>&) const;

private:
	RenderTargetRef renderTarget;
	std::vector<std::shared_ptr<GeometryModel>> geometryModels;
};