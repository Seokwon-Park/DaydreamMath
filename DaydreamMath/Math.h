#pragma once

#include "BaseTypes.h"

namespace Daydream
{
	namespace Math
	{
		template <typename T>
		[[nodiscard]] inline T Min(const T& _a, const T& _b)
		{
			return (_a < _b) ? _a : _b;
		}

		template <typename T>
		[[nodiscard]] inline T Max(const T& _a, const T& _b)
		{
			return (_a > _b) ? _a : _b;
		}

		template <typename T>
		[[nodiscard]] inline T Clamp(const T& _value, const T& _low, const T& _high)
		{
			if (_value < _low) return _low;
			if (_value > _high) return _high;
			return _value;
		}

		template <typename T>
		[[nodiscard]] inline T DegreeToRadian(const T& _degrees)
		{
			return _degrees * GenericDegToRad<T>;
		}

		template <typename T>
		[[nodiscard]] inline T RadianToDegree(const T& _radians)
		{
			return _radians * GenericRadToDeg<T>;
		}

		template <std::floating_point T>
		[[nodiscard]] inline T Cos(T _radian)
		{
			return std::cos(_radian);
		}

		template <std::floating_point T>
		[[nodiscard]] inline T CosDegree(T _degrees)
		{
			// 이전에 만든 템플릿 버전의 DegreeToRadian을 호출
			return std::cos(DegreeToRadian<T>(_degrees));
		}

		template <std::floating_point T>
		[[nodiscard]] inline Bool IsNearlyEqual(T _a, T _b, T _epsilon = Math::GenericEpsilon<T>)
		{
			return std::abs(_a - _b) <= _epsilon;
		}

		template <typename T, typename U>
		[[nodiscard]] inline T Lerp(const T& _a, const T& _b, const U& _t)
		{
			return _a + (_b - _a) * _t; // (1 - t) * a + b*t;
		}

		template <std::unsigned_integral T>
		inline T BitCount(T _x)
		{
			return std::popcount(_x);
		}
	}
}