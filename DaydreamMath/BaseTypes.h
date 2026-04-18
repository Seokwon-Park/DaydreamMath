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

	namespace Math
	{
		template <std::floating_point T>
		constexpr T GenericPI = static_cast<T>(std::numbers::pi);

		template <std::floating_point T>
		constexpr T GenericHalfPI = GenericPI<T> *static_cast<T>(0.5);

		template <std::floating_point T>
		constexpr T GenericTwoPI = GenericPI<T> *static_cast<T>(2.0);

		template <std::floating_point T>
		constexpr T GenericEpsilon = std::numeric_limits<T>::min();

		template <std::floating_point T>
		constexpr T GenericDegToRad = GenericPI<T> / static_cast<T>(180.0);

		template <std::floating_point T>
		constexpr T GenericRadToDeg = static_cast<T>(180.0) / GenericPI<T>;

		constexpr Float32 PI = GenericPI<Float32>;
		constexpr Float32 HalfPI = GenericHalfPI<Float32>;
		constexpr Float32 TwoPI = GenericTwoPI<Float32>;
		constexpr Float32 Epsilon = GenericEpsilon<Float32>;
		constexpr Float32 DegToRad = GenericDegToRad<Float32>;
		constexpr Float32 RadToDeg = GenericRadToDeg<Float32>;

		constexpr Float64 PID = GenericPI<Float64>;
		constexpr Float64 HalfPID = GenericHalfPI<Float64>;
		constexpr Float64 TwoPID = GenericTwoPI<Float64>;
		constexpr Float64 EpsilonD = static_cast<Float64>(1e-8);
		constexpr Float64 DegToRadD = GenericDegToRad<Float64>;
		constexpr Float64 RadToDegD = GenericRadToDeg<Float64>;
	}
}