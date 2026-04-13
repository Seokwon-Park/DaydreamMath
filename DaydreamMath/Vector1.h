#pragma once

#include "Vector.h"

namespace Daydream
{
	template<typename T>
	struct alignas(16) Vector<1, T>
	{
		T x;
	};
}