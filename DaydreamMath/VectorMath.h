#pragma once

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

namespace Daydream
{
	namespace Math
	{
		template <UInt64 N, typename T>
		[[nodiscard]] inline Vector<N, T> Min(const Vector<N, T>& _a, const Vector<N, T>& _b)
		{
			Vector<N, T> result;
			for (UInt64 i = 0; i < N; i++) result[i] = Math::Min(_a[i], _b[i]);
			return result;
		}

		template <UInt64 N, typename T>
		[[nodiscard]] inline Vector<N, T> Max(const Vector<N, T>& _a, const Vector<N, T>& _b)
		{
			Vector<N, T> result;
			for (UInt64 i = 0; i < N; i++) result[i] = Math::Max(_a[i], _b[i]);
			return result;
		}

		template <UInt64 N, typename T>
		[[nodiscard]] inline Vector<N, T> Clamp(const Vector<N, T>& _v, const Vector<N, T>& _min, const Vector<N, T>& _max)
		{
			Vector<N, T> result;
			for (UInt64 i = 0; i < N; i++) result[i] = Math::Clamp(_v[i], _min[i], _max[i]);
			return result;
		}

		template <UInt64 N, std::floating_point T>
		[[nodiscard]] inline Vector<N, T> DegreeToRadian(const Vector<N, T>& _degrees)
		{
			Vector<N, T> result;
			for (UInt64 i = 0; i < N; i++) result[i] = _degrees[i] * GenericDegToRad<T>;
			return result;
		}

		template <UInt64 N, std::floating_point T>
		[[nodiscard]] inline Vector<N, T> RadianToDegree(const Vector<N, T>& _radians)
		{
			Vector<N, T> result;
			for (UInt64 i = 0; i < N; i++) result[i] = _radians[i] * GenericDegToRad<T>;
			return result;
		}

		template <UInt64 N, std::floating_point T>
		[[nodiscard]] inline Vector<N, T> Cos(const Vector<N, T>& _radians)
		{
			Vector<N, T> result;
			for (UInt64 i = 0; i < N; i++) result[i] = std::cos(_radians[i]);
			return result;
		}

		template <UInt64 N, std::floating_point T>
		[[nodiscard]] inline Vector<N, T> CosDegree(const Vector<N, T>& _degrees)
		{
			Vector<N, T> result;
			for (UInt64 i = 0; i < N; i++) result[i] = CosDegree(_degrees[i]);
			return result;
		}

		template <UInt64 N, std::floating_point T>
		[[nodiscard]] inline Bool IsNearlyEqual(const Vector<N, T>& _a, const Vector<N, T>& _b, T _epsilon = Math::GenericEpsilon<T>)
		{
			for (UInt64 i = 0; i < N; i++)
			{
				if (std::abs(_a[i] - _b[i]) > _epsilon)
				{
					return false;
				}
			}
			return true;
		}

		template<UInt64 N, typename T>
		[[nodiscard]] inline T Dot(const Vector<N, T>& _a, const Vector<N, T>& _b)
		{
			T result = 0;
			for (int i = 0; i < N; ++i) result += _a[i] * _b[i];
			return result;
		}

		template<UInt64 N, typename T>
		[[nodiscard]] inline T LengthSq(const Vector<N, T>& _vector)
		{
			return Dot(_vector, _vector);
		}

		template<UInt64 N, typename T>
		[[nodiscard]] inline T Length(const Vector<N, T>& _vector)
		{
			return static_cast<T>(std::sqrt(static_cast<Float64>(LengthSq(_vector))));
		}

		template<UInt64 N, typename T>
		[[nodiscard]] inline Vector<N, T> Normalized(const Vector<N, T>& _vector)
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

		template<typename T>
		[[nodiscard]] inline Vector<3,T> RotateX(const Vector<3, T>& _vector, Float32 _radian)
		{
			Vector<3, T> result = _vector;
			result.y = _vector.y * std::cos(_radian) - _vector.z * std::sin(_radian);
			result.z = _vector.y * std::sin(_radian) + _vector.z * std::cos(_radian);
			return result;
		}

		template<typename T>
		[[nodiscard]] inline Vector<3, T> RotateY(const Vector<3, T>& _vector, Float32 _radian)
		{
			Vector<3, T> result = _vector;
			result.z = _vector.z * std::cos(_radian) - _vector.x * std::sin(_radian);
			result.x = _vector.z * std::sin(_radian) + _vector.x * std::cos(_radian);
			return result;
		}

		template<typename T>
		[[nodiscard]] inline Vector<3, T> RotateZ(const Vector<3, T>& _vector, Float32 _radian)
		{
			Vector<3, T> result = _vector;
			result.x = _vector.x * std::cos(_radian) - _vector.y * std::sin(_radian);
			result.y = _vector.x * std::sin(_radian) + _vector.y * std::cos(_radian);
			return result;
		}

		template<typename T>
		[[nodiscard]] inline Vector<3, T> RotateXDeg(const Vector<3, T>& _vector, Float32 _degree)
		{
			return RotateX(_vector, DegToRad * _degree);
		}

		template<typename T>
		[[nodiscard]] inline Vector<3, T> RotateYDeg(const Vector<3, T>& _vector, Float32 _degree)
		{
			return RotateY(_vector, DegToRad * _degree);
		}

		template<typename T>
		[[nodiscard]] inline Vector<3, T> RotateZDeg(const Vector<3, T>& _vector, Float32 _degree)
		{
			return RotateZ(_vector, DegToRad * _degree);
		}
	}
}
