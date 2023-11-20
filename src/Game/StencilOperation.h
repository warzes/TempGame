#pragma once

namespace graphics
{
	enum class StencilOperation
	{
		keep,
		zero,
		replace,
		incrementClamp,
		decrementClamp,
		invert,
		incrementWrap,
		decrementWrap
	};
}