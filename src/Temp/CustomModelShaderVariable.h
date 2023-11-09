#pragma once

#include "GenericRendererBuilder.h"

/**
* Allow to load custom model shader variables for model displayer
*/
class CustomModelShaderVariable {
public:
	virtual ~CustomModelShaderVariable() = default;

	virtual void setupMeshRenderer(const std::shared_ptr<GenericRendererBuilder>&) = 0;
	virtual void loadCustomShaderVariables() = 0;

};