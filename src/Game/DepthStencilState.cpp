#include "stdafx.h"
#include "DepthStencilState.h"

namespace graphics
{
	DepthStencilState::DepthStencilState(
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
		CompareFunction initBackFaceStencilCompareFunction) :
		depthTest{ initDepthTest },
		depthWrite{ initDepthWrite },
		compareFunction{ initCompareFunction },
		stencilEnabled{ initStencilEnabled },
		stencilReadMask{ initStencilReadMask },
		stencilWriteMask{ initStencilWriteMask },
		frontFaceStencilFailureOperation{ initFrontFaceStencilFailureOperation },
		frontFaceStencilDepthFailureOperation{ initFrontFaceStencilDepthFailureOperation },
		frontFaceStencilPassOperation{ initFrontFaceStencilPassOperation },
		frontFaceStencilCompareFunction{ initFrontFaceStencilCompareFunction },
		backFaceStencilFailureOperation{ initBackFaceStencilFailureOperation },
		backFaceStencilDepthFailureOperation{ initBackFaceStencilDepthFailureOperation },
		backFaceStencilPassOperation{ initBackFaceStencilPassOperation },
		backFaceStencilCompareFunction{ initBackFaceStencilCompareFunction }
	{
		//initGraphics.addCommand(std::make_unique<InitDepthStencilStateCommand>(resource,
		//	initDepthTest,
		//	initDepthWrite,
		//	initCompareFunction,
		//	initStencilEnabled,
		//	initStencilReadMask,
		//	initStencilWriteMask,
		//	initFrontFaceStencilFailureOperation,
		//	initFrontFaceStencilDepthFailureOperation,
		//	initFrontFaceStencilPassOperation,
		//	initFrontFaceStencilCompareFunction,
		//	initBackFaceStencilFailureOperation,
		//	initBackFaceStencilDepthFailureOperation,
		//	initBackFaceStencilPassOperation,
		//	initBackFaceStencilCompareFunction));
	}
}