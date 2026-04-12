#pragma once

#include "MathCommon.h"

#include <array>
#include <cassert>
#include <cmath>
#include <initializer_list>

namespace Daydream::Math {

template <std::size_t N, typename T = Float32>
class Vector final
{
public:
    using ValueType = T;

    Vector() = default;
    explicit Vector(const T scalar) { values_.fill(scalar); }

    Vector(std::initializer_list<T> values)
    {
        assert(values.size() == N && "initializer size must match vector dimension");
        std::size_t index = 0;
        for (const auto value : values)
        {
            values_[index++] = value;
        }
    }

    [[nodiscard]] static Vector Zero()
    {
        return Vector{};
    }

    [[nodiscard]] static Vector One()
    {
        return Vector{static_cast<T>(1)};
    }

    [[nodiscard]] constexpr std::size_t Size() const { return N; }

    T& operator[](const std::size_t index)
    {
        return values_[index];
    }

    const T& operator[](const std::size_t index) const
    {
        return values_[index];
    }

    Vector& operator+=(const Vector& rhs)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            values_[i] += rhs.values_[i];
        }
        return *this;
    }

    Vector& operator-=(const Vector& rhs)
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            values_[i] -= rhs.values_[i];
        }
        return *this;
    }

    Vector& operator*=(const T scalar)
    {
        for (auto& value : values_)
        {
            value *= scalar;
        }
        return *this;
    }

    [[nodiscard]] T LengthSquared() const
    {
        T result{};
        for (const auto value : values_)
        {
            result += value * value;
        }
        return result;
    }

    [[nodiscard]] T Length() const
    {
        return static_cast<T>(std::sqrt(LengthSquared()));
    }

    [[nodiscard]] Vector Normalized() const
    {
        const auto len = Length();
        if (NearlyEqual(static_cast<Float32>(len), 0.0f))
        {
            return Vector::Zero();
        }
        return (*this) * (static_cast<T>(1) / len);
    }

private:
    std::array<T, N> values_{};
};

template <std::size_t N, typename T>
[[nodiscard]] inline Vector<N, T> operator+(Vector<N, T> lhs, const Vector<N, T>& rhs)
{
    lhs += rhs;
    return lhs;
}

template <std::size_t N, typename T>
[[nodiscard]] inline Vector<N, T> operator-(Vector<N, T> lhs, const Vector<N, T>& rhs)
{
    lhs -= rhs;
    return lhs;
}

template <std::size_t N, typename T>
[[nodiscard]] inline Vector<N, T> operator*(Vector<N, T> lhs, const T scalar)
{
    lhs *= scalar;
    return lhs;
}

template <std::size_t N, typename T>
[[nodiscard]] inline Vector<N, T> operator*(const T scalar, Vector<N, T> rhs)
{
    rhs *= scalar;
    return rhs;
}

template <std::size_t N, typename T>
[[nodiscard]] inline T Dot(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
    T result{};
    for (std::size_t i = 0; i < N; ++i)
    {
        result += lhs[i] * rhs[i];
    }
    return result;
}

[[nodiscard]] inline Vector<3, Float32> Cross(const Vector<3, Float32>& lhs, const Vector<3, Float32>& rhs)
{
    return Vector<3, Float32>{
        lhs[1] * rhs[2] - lhs[2] * rhs[1],
        lhs[2] * rhs[0] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - lhs[1] * rhs[0]
    };
}

using Vec2 = Vector<2, Float32>;
using Vec3 = Vector<3, Float32>;
using Vec4 = Vector<4, Float32>;

} // namespace Daydream::Math
