#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>
#include <bit>
#include <cassert>
#include <initializer_list>

namespace Daydream
{
	// unsigned integers
	using UInt64 = std::uint64_t;
	using UInt32 = std::uint32_t;
	using UInt16 = std::uint16_t;
	using UInt8 = std::uint8_t;

	// signed integers
	using Int64 = std::int64_t;
	using Int32 = std::int32_t;
	using Int16 = std::int16_t;
	using Int8 = std::int8_t;

	using Bool = bool;

	using Float32 = float;
	using Float64 = double;

	// 템플릿 상수 정의 (기본값은 Float32)
	template <typename T = Float32>
	constexpr T PI = static_cast<T>(std::numbers::pi);

	template <typename T = Float32>
	constexpr T HalfPI = PI<T> *static_cast<T>(0.5);

	template <typename T = Float32>
	constexpr T TwoPI = PI<T> *static_cast<T>(2.0);

	template <typename T = Float32>
	constexpr T Epsilon = static_cast<T>(1e-5);

	template <typename T = Float32>
	constexpr T DegToRad = PI<T> / static_cast<T>(180.0);

	template <typename T = Float32>
	constexpr T RadToDeg = static_cast<T>(180.0) / PI<T>;
}