#pragma once

#include "Fwd.h"
#include "SIMD.h"

namespace Daydream
{
	template<>
	struct alignas(16) Vector<4, Float32>
	{
		union
		{
			struct
			{
				Float32 x, y, z, w;
			};
			Float32 values[4];
			SIMDRegister reg;
		};

		Vector()
		{
			reg = SIMD::SetZero();
		}

		Vector(SIMDRegister _reg)
			: reg(_reg)
		{
		}

		Vector(Float32 _value)
		{
			reg = SIMD::SetVal(_value);
		}

		Vector(Float32 _x, Float32 _y, Float32 _z, Float32 _w)
		{
			reg = SIMD::SetVal(_x, _y, _z, _w);
		}

		template <typename X, typename Y, typename Z, typename W>
		Vector(X _x, Y _y, Z _z, W _w)
		{
			reg = SIMD::SetVal(static_cast<Float32>(_x), static_cast<Float32>(_y), static_cast<Float32>(_z), static_cast<Float32>(_w));
		};

		Vector(std::initializer_list<Float32> _values)
		{
			assert(_values.size() <= 4 && "initializer list size must be less than or equal to 4");

			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;

			auto it = _values.begin();
			if (it != _values.end()) x = *it++;
			if (it != _values.end()) y = *it++;
			if (it != _values.end()) z = *it++;
			if (it != _values.end()) w = *it++;
		}

		Float32& operator[](UInt64 _index) { return values[_index]; }
		const Float32& operator[](UInt64 _index) const { return values[_index]; }

		inline Vector operator-() const { return Vector(SIMD::Sub(SIMD::SetZero(), reg)); }
		inline Vector operator+(const Vector& _other) const { return Vector(SIMD::Add(reg, _other.reg)); }
		inline Vector operator-(const Vector& _other) const { return Vector(SIMD::Sub(reg, _other.reg)); }
		inline Vector operator*(const Vector& _other) const { return Vector(SIMD::Mul(reg, _other.reg)); }
		inline Vector operator/(const Vector& _other) const { return Vector(SIMD::Div(reg, _other.reg)); }

		inline Vector operator+(Float32 _scalar) const { return Vector(SIMD::Add(reg, SIMD::SetVal(_scalar))); }
		inline Vector operator-(Float32 _scalar) const { return Vector(SIMD::Sub(reg, SIMD::SetVal(_scalar))); }
		inline Vector operator*(Float32 _scalar) const { return Vector(SIMD::Mul(reg, SIMD::SetVal(_scalar))); }
		inline Vector operator/(Float32 _scalar) const { return Vector(SIMD::Div(reg, SIMD::SetVal(_scalar))); }

		inline Vector& operator+=(const Vector& _other) { reg = SIMD::Add(reg, _other.reg); return *this; }
		inline Vector& operator-=(const Vector& _other) { reg = SIMD::Sub(reg, _other.reg); return *this; }
		inline Vector& operator*=(const Vector& _other) { reg = SIMD::Mul(reg, _other.reg); return *this; }
		inline Vector& operator/=(const Vector& _other) { reg = SIMD::Div(reg, _other.reg); return *this; }

		inline Vector& operator+=(Float32 _scalar) { reg = SIMD::Add(reg, SIMD::SetVal(_scalar)); return *this; }
		inline Vector& operator-=(Float32 _scalar) { reg = SIMD::Sub(reg, SIMD::SetVal(_scalar)); return *this; }
		inline Vector& operator*=(Float32 _scalar) { reg = SIMD::Mul(reg, SIMD::SetVal(_scalar)); return *this; }
		inline Vector& operator/=(Float32 _scalar) { reg = SIMD::Div(reg, SIMD::SetVal(_scalar)); return *this; }

		bool operator==(const Vector& _other) const { return x == _other.x && y == _other.y && z == _other.z && w == _other.w; }
		bool operator!=(const Vector& _other) const { return !(*this == _other); }

		[[nodiscard]] static Vector Min(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Max(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Clamp(const Vector& _vector, const Vector& _min, const Vector& _max);
		[[nodiscard]] static Bool IsNearlyEqual(const Vector& _a, const Vector& _b, Float32 _epsilon = Math::Epsilon<Float32>);

		[[nodiscard]] Float32 LengthSq() const;
		[[nodiscard]] Float32 Length() const;
		[[nodiscard]] static Float32 LengthSq(const Vector& _vector);
		[[nodiscard]] static Float32 Length(const Vector& _vector);

		[[nodiscard]] Vector Normalized() const;
		[[nodiscard]] static Vector Normalized(const Vector& _vector);
		void Normalize();

		[[nodiscard]] static Float32 Dot(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Float32 DistanceSq(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Float32 Distance(const Vector& _a, const Vector& _b);
	};

}