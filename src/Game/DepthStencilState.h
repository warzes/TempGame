#pragma once

#include "CompareFunction.h"
#include "StencilOperation.h"

namespace graphics
{
	class Graphics;

	class DepthStencilState final
	{
	public:
		DepthStencilState() = default;
		DepthStencilState(
			bool initDepthTest,
			bool initDepthWrite,
			CompareFunction initCompareFunction,
			bool initStencilEnabled,
			std::uint32_t initStencilReadMask,
			std::uint32_t initStencilWriteMask,
			StencilOperation initFrontFaceStencilFailureOperation,
			StencilOperation initFrontFaceStencilDepthFailureOperation,
			StencilOperation initFrontFaceStencilPassOperation,
			CompareFunction initFrontFaceStencilCompareFunction,
			StencilOperation initBackFaceStencilFailureOperation,
			StencilOperation initBackFaceStencilDepthFailureOperation,
			StencilOperation initBackFaceStencilPassOperation,
			CompareFunction initBackFaceStencilCompareFunction);

		auto& getResource() const noexcept { return resource; }

	private:
		RenderDevice::Resource resource;

		bool depthTest = false;
		bool depthWrite = false;
		CompareFunction compareFunction;
		bool stencilEnabled = false;
		std::uint32_t stencilReadMask = 0xFFFFFFFFU;
		std::uint32_t stencilWriteMask = 0xFFFFFFFFU;
		StencilOperation frontFaceStencilFailureOperation;
		StencilOperation frontFaceStencilDepthFailureOperation;
		StencilOperation frontFaceStencilPassOperation;
		CompareFunction frontFaceStencilCompareFunction;
		StencilOperation backFaceStencilFailureOperation;
		StencilOperation backFaceStencilDepthFailureOperation;
		StencilOperation backFaceStencilPassOperation;
		CompareFunction backFaceStencilCompareFunction;
	};
}
