#pragma once

#include "Vector4SIMD.h"

namespace Daydream
{

	Vector4 Vector4::Min(const Vector& _a, const Vector& _b)
	{
		return SIMD::Min(_a.reg, _b.reg);
	}

	Vector4 Vector4::Max(const Vector& _a, const Vector& _b)
	{
		return SIMD::Max(_a.reg, _b.reg);
	}

	Vector4 Vector4::Clamp(const Vector& _vector, const Vector& _min, const Vector& _max)
	{
		// 1. _vector와 _min 중 더 '큰' 값을 고른다 (하한선 방어)
		SIMDRegister clampedBottom = SIMD::Max(_vector.reg, _min.reg);

		// 2. 그 결과와 _max 중 더 '작은' 값을 고른다 (상한선 방어)
		SIMDRegister finalResult = SIMD::Min(clampedBottom, _max.reg);

		return Vector(finalResult);
	}

	inline Bool Vector<4, Float32>::IsNearlyEqual(const Vector& _a, const Vector& _b, Float32 _epsilon)
	{
		return SIMD::IsNearlyEqual(_a.reg, _b.reg, _epsilon);
	}

	inline Float32 Vector4::LengthSq() const
	{
		return SIMD::Dot(reg, reg);
	}

	inline Float32 Vector4::Length() const
	{
		return std::sqrt(LengthSq());
	}

	Float32  Vector4::LengthSq(const Vector& _vector)
	{
		return _vector.LengthSq();
	}

	Float32  Vector4::Length(const Vector& _vector)
	{
		return _vector.Length();
	}

	Vector4 Vector4::Normalized() const
	{
		Float32 lenSq = LengthSq();

		// 치명적인 버그 방어: 길이가 0이거나 0에 한없이 가까울 때 
		// 나눗셈을 시도하면 NaN(Not a Number)이 발생하여 엔진이 터집니다.
		// 1e-8f 대신 미리 정의해두신 Epsilon 상수를 쓰셔도 좋습니다.
		if (lenSq < 1e-8f)
		{
			return Vector4(SIMD::SetZero());
		}

		Float32 len = std::sqrt(lenSq);

		// SIMD 나눗셈 명령어 하나로 x, y, z, w 4개의 성분을 동시에 len으로 나눕니다.
		return Vector4(SIMD::Div(reg, SIMD::SetVal(len)));
	}

	Vector4 Vector4::Normalized(const Vector& _vector)
	{
		return _vector.Normalized();
	}

	void Vector4::Normalize()
	{
		reg = Normalized().reg;
	}

	[[nodiscard]] Float32 Vector4::Dot(const Vector& _a, const Vector& _b)
	{
		return SIMD::Dot(_a.reg, _b.reg);
	}

	Float32 Vector4::DistanceSq(const Vector& _a, const Vector& _b)
	{
		SIMDRegister diff = SIMD::Sub(_a.reg, _b.reg);
		return SIMD::Dot(diff, diff);
	}
	Float32 Vector4::Distance(const Vector& _a, const Vector& _b)
	{
		// 거리의 제곱에 표준 루트 함수를 씌웁니다.
		return std::sqrt(DistanceSq(_a, _b));
	}

}