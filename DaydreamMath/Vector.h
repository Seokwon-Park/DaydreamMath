#pragma once

#include "Common.h"

#include <array>
#include <cassert>
#include <cmath>
#include <initializer_list>

namespace Daydream
{
	template <UInt64 N, typename T = Float32> struct Vector;
	//class Vector final
	//{
	//public:
	//	static_assert(N >= 2, "Vector dimension must be at least 2.");

	//	Vector() = default;
	//	explicit Vector(const ValueType _scalar) { values.fill(_scalar); }

	//	Vector(std::initializer_list<ValueType> _values)
	//	{
	//		assert(values.size() == N && "initializer size must match vector dimension");
	//		UInt64 index = 0;
	//		for (const auto& value : values)
	//		{
	//			values[index++] = value;
	//		}
	//	}

	//	[[nodiscard]] static Vector Zero()
	//	{
	//		return Vector{};
	//	}

	//	[[nodiscard]] static Vector One()
	//	{
	//		return Vector{ static_cast<ValueType>(1) };
	//	}

	//	[[nodiscard]] constexpr UInt64 Size() const { return N; }

	//	ValueType& operator[](const UInt64 _index)
	//	{
	//		return values[index];
	//	}

	//	const ValueType& operator[](const UInt64 index) const
	//	{
	//		return values[index];
	//	}

	//	Vector& operator+=(const Vector& _rhs)
	//	{
	//		for (std::size_t i = 0; i < N; ++i)
	//		{
	//			values[i] += _rhs.values[i];
	//		}
	//		return *this;
	//	}

	//	Vector& operator-=(const Vector& _rhs)
	//	{
	//		for (std::size_t i = 0; i < N; ++i)
	//		{
	//			values[i] -= _rhs.values[i];
	//		}
	//		return *this;
	//	}

	//	Vector& operator*=(const ValueType _scalar)
	//	{
	//		for (auto& value : values)
	//		{
	//			value *= scalar;
	//		}
	//		return *this;
	//	}

	//	[[nodiscard]] ValueType LengthSquared() const
	//	{
	//		ValueType result{};
	//		for (const auto value : values)
	//		{
	//			result += value * value;
	//		}
	//		return result;
	//	}

	//	[[nodiscard]] ValueType Length() const
	//	{
	//		return static_cast<ValueType>(std::sqrt(LengthSquared()));
	//	}

	//	[[nodiscard]] Vector Normalized() const
	//	{
	//		const auto len = Length();
	//		if (NearlyEqual(static_cast<Float32>(len), 0.0f))
	//		{
	//			return Vector::Zero();
	//		}
	//		return (*this) * (static_cast<ValueType>(1) / len);
	//	}

	//private:
	//	std::array<ValueType, N> values{};
	//};

	//template <std::size_t N, typename ValueType>
	//[[nodiscard]] inline Vector<N, ValueType> operator+(Vector<N, ValueType> lhs, const Vector<N, ValueType>& rhs)
	//{
	//	lhs += rhs;
	//	return lhs;
	//}

	//template <std::size_t N, typename ValueType>
	//[[nodiscard]] inline Vector<N, ValueType> operator-(Vector<N, ValueType> lhs, const Vector<N, ValueType>& rhs)
	//{
	//	lhs -= rhs;
	//	return lhs;
	//}

	//template <std::size_t N, typename ValueType>
	//[[nodiscard]] inline Vector<N, ValueType> operator*(Vector<N, ValueType> lhs, const ValueType scalar)
	//{
	//	lhs *= scalar;
	//	return lhs;
	//}

	//template <std::size_t N, typename ValueType>
	//[[nodiscard]] inline Vector<N, ValueType> operator*(const ValueType scalar, Vector<N, ValueType> rhs)
	//{
	//	rhs *= scalar;
	//	return rhs;
	//}

	//template <std::size_t N, typename ValueType>
	//[[nodiscard]] inline ValueType Dot(const Vector<N, ValueType>& lhs, const Vector<N, ValueType>& rhs)
	//{
	//	ValueType result{};
	//	for (std::size_t i = 0; i < N; ++i)
	//	{
	//		result += lhs[i] * rhs[i];
	//	}
	//	return result;
	//}

	//[[nodiscard]] inline Vector<3, Float32> Cross(const Vector<3, Float32>& lhs, const Vector<3, Float32>& rhs)
	//{
	//	return Vector<3, Float32>
	//	{
	//		lhs[1] * rhs[2] - lhs[2] * rhs[1],
	//			lhs[2] * rhs[0] - lhs[0] * rhs[2],
	//			lhs[0] * rhs[1] - lhs[1] * rhs[0]
	//	};
	//}

	//using Vector2 = Vector<2, Float32>;
	//using Vector3 = Vector<3, Float32>;
	//using Vector4 = Vector<4, Float32>;

} // namespace Daydream::Math
