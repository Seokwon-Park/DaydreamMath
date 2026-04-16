#pragma once

#include "Fwd.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "SIMD.h"

namespace Daydream
{
	template <UInt64 N, typename T>
	[[nodiscard]] inline Vector<N, T> Min(const Vector<N, T>& _a, const Vector<N, T>& _b)
	{
		Vector<N, T> result;
		for (UInt64 i = 0; i < N; i++) result[i] = Min(_a[i], _b[i]);
		return result;
	}

	template <UInt64 N, typename T>
	[[nodiscard]] inline Vector<N, T> Max(const Vector<N, T>& _a, const Vector<N, T>& _b)
	{
		Vector<N, T> result;
		for (UInt64 i = 0; i < N; i++) result[i] = Max(_a[i], _b[i]);
		return result;
	}

	template <UInt64 N, typename T>
	[[nodiscard]] inline Vector<N, T> Clamp(const Vector<N, T>& _v, const Vector<N, T>& _min, const Vector<N, T>& _max)
	{
		Vector<N, T> result;
		for (UInt64 i = 0; i < N; i++) result[i] = Clamp(_v[i], _min[i], _max[i]);
		return result;
	}

	template<UInt64 N, typename T>
	inline T Dot(const Vector<N, T>& _a, const Vector<N, T>& _b)
	{
		T result = 0;
		for (int i = 0; i < N; ++i) result += _a[i] * _b[i];
		return result;
	}

	template<>
	inline Float32 Dot(const Vector<4, Float32>& _a, const Vector<4, Float32>& _b)
	{
		return SIMD::Dot(_a.reg, _b.reg);
	}

	template<UInt64 N, typename T>
	inline T LengthSq(const Vector<N, T>& _vector)
	{
		return Dot(_vector, _vector);
	}

	template<UInt64 N, typename T>
	inline T Length(const Vector<N, T>& _vector)
	{
		return static_cast<T>(std::sqrt(static_cast<Float64>(LengthSq(_vector))));
	}

	template<UInt64 N, typename T>
	inline Vector<N, T> Normalized(const Vector<N, T>& _vector)
	{
		const T length = Length(_vector);
		if (length == static_cast<T>(0))
		{
			return Vector<N, T>{};
		}

		Vector<N, T> result{};
		const T inv = static_cast<T>(1.0) / length;
		for (UInt64 i = 0; i < N; ++i)
		{
			result.values[i] = _vector.values[i] * inv;
		}
		return result;
	}

	template<UInt64 N, typename T>
	inline void Normalize(Vector<N, T>& _vector)
	{
		_vector = Normalized(_vector);
	}

	template <typename T>
	inline Vector<3, T> operator*(const Vector<3, T>& _v, const Matrix<4, 4, T>& _m) = delete;

	Vector3 RotateVector(const Quaternion& _quat, const Vector3& _vector)
	{
		return Vector3();
	}

}
