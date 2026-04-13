#pragma once

#include "Fwd.h"
#include "SIMD.h"

namespace Daydream
{
	template<typename T>
	struct alignas(16) Vector<4, T>
	{
		T values[4];
	};

	template<>
	struct alignas(16) Vector<4, Float32>
	{
		union
		{
			struct
			{
				Float32 x, y, z, w;
			};
			Float32 values[4];
			SIMDRegister reg;
		};
	};
}
