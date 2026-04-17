#pragma once

#include "Fwd.h"

namespace Daydream
{
	template<typename T>
	struct Vector<2, T>
	{
		union
		{
			struct
			{
				T x, y;
			};
			T values[2];
		};

		Vector()
			: x(static_cast<T>(0)), y(static_cast<T>(0))
		{
		};

		Vector(T _x, T _y)
			: x(_x), y(_y)
		{
		};

		Vector(std::initializer_list<T> _values)
		{
			assert(_values.size() <= 2 && "initializer list size must be less than or equal to 2");

			x = static_cast<T>(0);
			y = static_cast<T>(0);

			auto it = _values.begin();
			if (it != _values.end()) x = *it++;
			if (it != _values.end()) y = *it++;
		}

		Vector& operator=(std::initializer_list<T> _values)
		{
			assert(_values.size() <= 2 && "initializer list size must be less than or equal to 2");

			x = static_cast<T>(0);
			y = static_cast<T>(0);

			auto it = _values.begin();
			if (it != _values.end()) x = *it++;
			if (it != _values.end()) y = *it++;

			return *this;
		}

		T& operator[](UInt64 _index) { return values[_index]; }
		const T& operator[](UInt64 _index) const { return values[_index]; }

		Vector operator-() const { return Vector(-x, -y); }
		Vector operator+(const Vector& _other) const { return Vector(x + _other.x, y + _other.y); }
		Vector operator-(const Vector& _other) const { return Vector(x - _other.x, y - _other.y); }
		Vector operator*(const Vector& _other) const { return Vector(x * _other.x, y * _other.y); }
		Vector operator/(const Vector& _other) const { return Vector(x / _other.x, y / _other.y); }

		Vector operator+(T _scalar) const { return Vector(x + _scalar, y + _scalar); }
		Vector operator-(T _scalar) const { return Vector(x - _scalar, y - _scalar); }
		Vector operator*(T _scalar) const { return Vector(x * _scalar, y * _scalar); }
		Vector operator/(T _scalar) const
		{
			T inv = static_cast<T>(1.0) / _scalar;
			return Vector(x * inv, y * inv);
		}

		Vector& operator+=(const Vector& _other) { x += _other.x; y += _other.y; return *this; }
		Vector& operator-=(const Vector& _other) { x -= _other.x; y -= _other.y; return *this; }
		Vector& operator*=(const Vector& _other) { x *= _other.x; y *= _other.y; return *this; }
		Vector& operator/=(const Vector& _other) { x /= _other.x; y /= _other.y; return *this; }
		Vector& operator+=(T _scalar) { x += _scalar; y += _scalar; return *this; }
		Vector& operator-=(T _scalar) { x -= _scalar; y -= _scalar; return *this; }
		Vector& operator*=(T _scalar) { x *= _scalar; y *= _scalar; return *this; }
		Vector& operator/=(T _scalar)
		{
			T inv = static_cast<T>(1.0) / _scalar;
			x *= inv;
			y *= inv;
			return *this;
		}

		bool operator==(const Vector& _other) const { return x == _other.x && y == _other.y; }
		bool operator!=(const Vector& _other) const { return !(*this == _other); }

		[[nodiscard]] static Vector Min(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Max(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Clamp(const Vector& _vector, const Vector& _min, const Vector& _max);
		[[nodiscard]] static Bool IsNearlyEqual(const Vector& _a, const Vector& _b, T _epsilon = Math::GenericEpsilon<T>);

		[[nodiscard]] T LengthSq() const;
		[[nodiscard]] T Length() const;
		[[nodiscard]] static T LengthSq(const Vector& _vector);
		[[nodiscard]] static T Length(const Vector& _vector);

		[[nodiscard]] Vector Normalized() const;
		[[nodiscard]] static Vector Normalized(const Vector& _vector);
		void Normalize();

		[[nodiscard]] static T Dot(const Vector& _a, const Vector& _b);
		[[nodiscard]] static T Cross(const Vector& _a, const Vector& _b);
		[[nodiscard]] static T DistanceSq(const Vector& _a, const Vector& _b);
		[[nodiscard]] static T Distance(const Vector& _a, const Vector& _b);
	};


	template<typename T>
	inline Vector<2, T> operator+(T _scalar, const Vector<2, T>& _vector)
	{
		return _vector + _scalar;
	}

	template<typename T>
	inline Vector<2, T> operator-(T _scalar, const Vector<2, T>& _vector)
	{
		return Vector<2, T>(_scalar - _vector.x, _scalar - _vector.y);
	}

	template<typename T>
	inline Vector<2, T> operator*(T _scalar, const Vector<2, T>& _vector)
	{
		return _vector * _scalar;
	}

	template<typename T>
	inline Vector<2, T> operator/(T _scalar, const Vector<2, T>& _vector)
	{
		return Vector<2, T>(_scalar / _vector.x, _scalar / _vector.y);
	}
}