#pragma once

#include "Vector.h"

namespace Daydream
{
	template<typename T>
	struct alignas(16) Vector<3, T>
	{
		union
		{
			struct
			{
				T x, y, z;
			};
			T values[4];
		};
	};
}