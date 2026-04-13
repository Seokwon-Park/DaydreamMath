#pragma once

#include "SIMD.h"
#include "Vector.h"

namespace Daydream
{
	struct alignas(16) Color
	{
		union
		{
			struct
			{
				Float32 r, g, b, a;
			};
			Float32 color[4];
			SIMDRegister reg;
		};

		Vector<4> ToVector4() const
		{
			Vector<4> result;
			result.reg = reg; // æÀ∏Õ¿Ã(16πŸ¿Ã∆Æ)∏∏ ΩÔ ª©º≠ ¿¸¥ﬁ
			return result;
		}

		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color White;
	};
}