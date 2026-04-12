#pragma once

#include <cmath>
#include <cstdint>
#include <limits>
#include <numbers>

namespace Daydream::Math {

// Integer aliases
using UInt8 = std::uint8_t;
using UInt16 = std::uint16_t;
using UInt32 = std::uint32_t;
using UInt64 = std::uint64_t;

using Int8 = std::int8_t;
using Int16 = std::int16_t;
using Int32 = std::int32_t;
using Int64 = std::int64_t;

using Float32 = float;
using Float64 = double;

constexpr Float32 kPi = static_cast<Float32>(std::numbers::pi_v<Float64>);
constexpr Float32 kHalfPi = kPi * 0.5f;
constexpr Float32 kTwoPi = kPi * 2.0f;
constexpr Float32 kEpsilon = 1e-5f;

[[nodiscard]] inline bool NearlyEqual(const Float32 a, const Float32 b, const Float32 epsilon = kEpsilon)
{
    return std::fabs(a - b) <= epsilon;
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

} // namespace Daydream::Math
