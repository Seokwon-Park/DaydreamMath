#pragma once

#include "BaseTypes.h"

namespace Daydream
{
	template <UInt64 N, typename T> struct Vector;
	template <UInt64 R, UInt64 C, typename T> struct Matrix;
	template <typename T> struct Quat;

	using Quaternion = Quat<Float32>;
	using QuaternionD = Quat<Float64>;

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