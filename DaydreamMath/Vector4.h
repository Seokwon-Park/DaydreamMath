#pragma once

#include "Vector.h"

namespace Daydream
{
	template<typename T>
	struct alignas(16) Vector<4, T>
	{
		union
		{
			struct
			{
				T x, y, z, w;
			};
			T values[4];
		};
	};
}