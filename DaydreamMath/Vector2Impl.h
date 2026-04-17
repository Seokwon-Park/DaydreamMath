#pragma once

#include "Vector3.h"
#include "VectorMath.h"

namespace Daydream
{
	template <typename T>
	inline Vector<2, T> Vector<2, T>::Min(const Vector& _a, const Vector& _b)
	{
		return Math::Min(_a, _b);
	}

	template <typename T>
	inline Vector<2, T> Vector<2, T>::Max(const Vector& _a, const Vector& _b)
	{
		return Math::Max(_a, _b);
	}

	template<typename T>
	inline Vector<2, T> Vector<2, T>::Clamp(const Vector& _vector, const Vector& _min, const Vector& _max)
	{
		return Math::Clamp(_vector, _min, _max);
	}

	template<typename T>
	Bool Vector<2, T>::IsNearlyEqual(const Vector& _a, const Vector& _b, T _epsilon)
	{
		return Math::IsNearlyEqual(_a, _b, _epsilon);
	}

	template <typename T>
	inline T Vector<2, T>::LengthSq() const
	{
		return Math::LengthSq(*this);
	}

	template <typename T>
	inline T Vector<2, T>::Length() const
	{
		return Math::Length(*this);
	}

	template <typename T>
	inline T Vector<2, T>::LengthSq(const Vector& _vector)
	{
		return Math::LengthSq(_vector);
	}

	template <typename T>
	inline T Vector<2, T>::Length(const Vector& _vector)
	{
		return Math::Length(_vector);
	}

	template<typename T>
	inline Vector<2, T> Vector<2, T>::Normalized(const Vector& _vector)
	{
		return Math::Normalized(_vector);
	}

	template <typename T>
	inline Vector<2, T> Vector<2, T>::Normalized() const
	{
		return Math::Normalized(*this);
	}

	template <typename T>
	inline void Vector<2, T>::Normalize()
	{
		*this = Math::Normalized(*this);
	}
	template <typename T>
	T Vector<2, T>::Dot(const Vector& _a, const Vector& _b)
	{
		return Math::Dot(_a, _b);
	}

	template<typename T>
	inline T Vector<2, T>::DistanceSq(const Vector& _a, const Vector& _b)
	{
		return (_a - _b).LengthSq();
	}

	template<typename T>
	inline T Vector<2, T>::Distance(const Vector& _a, const Vector& _b)
	{
		return (_a - _b).Length();
	}

	template <typename T>
	T Vector<2, T>::Cross(const Vector& _a, const Vector& _b)
	{
		return _a.x * _b.y - _a.y * _b.x;
	}
}