#pragma once

#include "Vector3.h"
#include "VectorMath.h"

namespace Daydream
{
	template <typename T>
	inline Vector<3, T> operator*(const Vector<3, T>& _v, const Matrix<4, 4, T>& _m) = delete;

	template <typename T>
	inline Vector<3, T> operator*(const Matrix<4, 4, T>& _m, const Vector<3, T>& _v) = delete;

	template <typename T>
	inline Vector<3, T> Vector<3, T>::Min(const Vector& _a, const Vector& _b)
	{
		return Math::Min(_a, _b);
	}

	template <typename T>
	inline Vector<3, T> Vector<3, T>::Max(const Vector& _a, const Vector& _b)
	{
		return Math::Max(_a, _b);
	}

	template<typename T>
	inline Vector<3, T> Vector<3, T>::Clamp(const Vector& _vector, const Vector& _min, const Vector& _max)
	{
		return Math::Clamp(_vector, _min, _max);
	}

	template<typename T>
	Bool Vector<3, T>::IsNearlyEqual(const Vector& _a, const Vector& _b, T _epsilon)
	{
		return Math::IsNearlyEqual(_a, _b, _epsilon);
	}

	template <typename T>
	inline T Vector<3, T>::LengthSq() const
	{
		return Math::LengthSq(*this);
	}

	template <typename T>
	inline T Vector<3, T>::Length() const
	{
		return Math::Length(*this);
	}

	template <typename T>
	inline T Vector<3, T>::LengthSq(const Vector& _vector)
	{
		return Math::LengthSq(_vector);
	}

	template <typename T>
	inline T Vector<3, T>::Length(const Vector& _vector)
	{
		return Math::Length(_vector);
	}

	template<typename T>
	inline Vector<3, T> Vector<3, T>::Normalized(const Vector& _vector)
	{
		return Math::Normalized(_vector);
	}

	template <typename T>
	inline Vector<3, T> Vector<3, T>::Normalized() const
	{
		return Math::Normalized(*this);
	}

	template <typename T>
	inline void Vector<3, T>::Normalize()
	{
		*this = Math::Normalized(*this);
	}
	template <typename T>
	T Vector<3, T>::Dot(const Vector& _a, const Vector& _b)
	{
		return Math::Dot(_a, _b);
	}

	template<typename T>
	inline T Vector<3, T>::DistanceSq(const Vector& _a, const Vector& _b)
	{
		return (_a - _b).LengthSq();
	}

	template<typename T>
	inline T Vector<3, T>::Distance(const Vector& _a, const Vector& _b)
	{
		return (_a - _b).Length();
	}

	template <typename T>
	Vector<3, T> Vector<3, T>::Cross(const Vector& _a, const Vector& _b)
	{
		return Vector(
			_a.y * _b.z - _a.z * _b.y,
			_a.z * _b.x - _a.x * _b.z,
			_a.x * _b.y - _a.y * _b.x
		);
	}


}
