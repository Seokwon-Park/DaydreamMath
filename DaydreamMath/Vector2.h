#pragma once

#include "Fwd.h"

namespace Daydream
{
	template<typename T>
	struct Vector<2, T>
	{
		union
		{
			struct
			{
				T x, y;
			};
			T values[2];
		};
	};
}