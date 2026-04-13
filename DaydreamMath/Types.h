#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>

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

	constexpr Float32 PI = static_cast<Float32>(std::numbers::pi);
	constexpr Float32 HalfPI = PI * 0.5f;
	constexpr Float32 TwoPI = PI * 2.0f;
	constexpr Float32 Epsilon = 1e-5f;

	[[nodiscard]] inline Bool IsNearlyEqual(const Float32 _a, const Float32 _b, const Float32 _epsilon = Epsilon)
	{
		return std::fabs(_a - _b) <= _epsilon;
	}

	[[nodiscard]] inline Float32 Clamp(const Float32 value, const Float32 low, const Float32 high)
	{
		if (value < low) return low;
		if (value > high) return high;
		return value;
	}

	[[nodiscard]] inline Float32 Lerp(const Float32 a, const Float32 b, const Float32 t)
	{
		return a + (b - a) * t;
	}
}