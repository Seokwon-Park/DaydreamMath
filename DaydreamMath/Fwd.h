#pragma once

#include <assert.h>
#include "Types.h"

namespace Daydream
{
	template <UInt64 N, typename T = Float32> struct Vector;
	template <UInt64 R, UInt64 C, typename T = Float32> struct Matrix;

	using Vector2 = Vector<2, Float32>;
	using Vector3 = Vector<3, Float32>;
	using Vector4 = Vector<4, Float32>;

	using Vector2D = Vector<2, Float64>;
	using Vector3D = Vector<3, Float64>;
	using Vector4D = Vector<4, Float64>;

	using Vector2I = Vector<2, Int32>;
	using Vector3I = Vector<3, Int32>;
	using Vector4I = Vector<4, Int32>;

	using Matrix4x4 = Matrix<4, 4, Float32>;
}