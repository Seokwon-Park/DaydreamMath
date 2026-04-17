#pragma once

#include "Fwd.h"

namespace Daydream
{
	template<typename T>
	struct Vector<3, T>
	{
		union
		{
			struct
			{
				T x, y, z;
			};
			T values[3];
		};

		Vector()
			: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0))
		{
		};

		Vector(T _value)
			: x(_value), y(_value), z(_value)
		{
		};

		Vector(T _x, T _y, T _z)
			: x(_x), y(_y), z(_z)
		{
		};

		template <typename X, typename Y, typename Z>
		Vector(X _x, Y _y, Z _z)
			: x(static_cast<T>(_x)), y(static_cast<T>(_y)), z(static_cast<T>(_z))
		{
		};

		Vector(std::initializer_list<T> _values)
		{
			assert(_values.size() <= 3 && "initializer list size must be less than or equal to 3");

			x = static_cast<T>(0);
			y = static_cast<T>(0);
			z = static_cast<T>(0);

			auto it = _values.begin();
			if (it != _values.end()) x = *it++;
			if (it != _values.end()) y = *it++;
			if (it != _values.end()) z = *it++;
		}

		Vector& operator=(std::initializer_list<T> _values)
		{
			assert(_values.size() <= 3 && "initializer list size must be less than or equal to 3");

			x = static_cast<T>(0);
			y = static_cast<T>(0);
			z = static_cast<T>(0);

			auto it = _values.begin();
			if (it != _values.end()) x = *it++;
			if (it != _values.end()) y = *it++;
			if (it != _values.end()) z = *it++;

			return *this;
		}

		T& operator[](UInt64 _index) { return values[_index]; }
		const T& operator[](UInt64 _index) const { return values[_index]; }

		inline Vector operator-() const { return Vector(-x, -y, -z); }
		inline Vector operator+(const Vector& _other) const { return Vector(x + _other.x, y + _other.y, z + _other.z); }
		inline Vector operator-(const Vector& _other) const { return Vector(x - _other.x, y - _other.y, z - _other.z); }
		inline Vector operator*(const Vector& _other) const { return Vector(x * _other.x, y * _other.y, z * _other.z); }
		inline Vector operator/(const Vector& _other) const { return Vector(x / _other.x, y / _other.y, z / _other.z); }

		inline Vector operator+(T _scalar) const { return Vector(x + _scalar, y + _scalar, z + _scalar); }
		inline Vector operator-(T _scalar) const { return Vector(x - _scalar, y - _scalar, z - _scalar); }
		inline Vector operator*(T _scalar) const { return Vector(x * _scalar, y * _scalar, z * _scalar); }
		inline Vector operator/(T _scalar) const
		{
			T inv = static_cast<T>(1.0) / _scalar;
			return Vector(x * inv, y * inv, z * inv);
		}

		inline Vector& operator+=(const Vector& _other) { x += _other.x; y += _other.y; z += _other.z; return *this; }
		inline Vector& operator-=(const Vector& _other) { x -= _other.x; y -= _other.y; z -= _other.z; return *this; }
		inline Vector& operator*=(const Vector& _other) { x *= _other.x; y *= _other.y; z *= _other.z; return *this; }
		inline Vector& operator/=(const Vector& _other) { x /= _other.x; y /= _other.y; z /= _other.z; return *this; }

		inline Vector& operator+=(T _scalar) { x += _scalar; y += _scalar; z += _scalar; return *this; }
		inline Vector& operator-=(T _scalar) { x -= _scalar; y -= _scalar; z -= _scalar; return *this; }
		inline Vector& operator*=(T _scalar) { x *= _scalar; y *= _scalar; z *= _scalar; return *this; }
		inline Vector& operator/=(T _scalar)
		{
			T inv = static_cast<T>(1.0) / _scalar;
			x *= inv;
			y *= inv;
			z *= inv;
			return *this;
		}

		bool operator==(const Vector& _other) const { return x == _other.x && y == _other.y && z == _other.z; }
		bool operator!=(const Vector& _other) const { return !(*this == _other); }

		[[nodiscard]] static Vector Min(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Max(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Clamp(const Vector& _vector, const Vector& _min, const Vector& _max);
		[[nodiscard]] static Vector DegreeToRadian(const Vector& _degrees);
		[[nodiscard]] static Vector RadianToDegree(const Vector& _radians);
		[[nodiscard]] static Vector Cos(const Vector& _radians);
		[[nodiscard]] static Vector CosDegree(const Vector& _degrees);
		[[nodiscard]] static Bool IsNearlyEqual(const Vector& _a, const Vector& _b, T _epsilon = Math::GenericEpsilon<T>);

		[[nodiscard]] T LengthSq() const;
		[[nodiscard]] T Length() const;
		[[nodiscard]] static T LengthSq(const Vector& _vector);
		[[nodiscard]] static T Length(const Vector& _vector);

		[[nodiscard]] Vector Normalized() const;
		[[nodiscard]] static Vector Normalized(const Vector& _vector);
		void Normalize();

		[[nodiscard]] static T Dot(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Cross(const Vector& _a, const Vector& _b);
		[[nodiscard]] static T DistanceSq(const Vector& _a, const Vector& _b);
		[[nodiscard]] static T Distance(const Vector& _a, const Vector& _b);
		[[nodiscard]] static Vector Rotate(const Quat<T>& _quat, const Vector _vector);
	};

	template<typename T>
	inline Vector<3, T> operator+(T _scalar, const Vector<3, T>& _vector)
	{
		return _vector + _scalar;
	}

	template<typename T>
	inline Vector<3, T> operator-(T _scalar, const Vector<3, T>& _vector)
	{
		return Vector<3, T>(_scalar - _vector.x, _scalar - _vector.y, _scalar - _vector.z);
	}

	template<typename T>
	inline Vector<3, T> operator*(T _scalar, const Vector<3, T>& _vector)
	{
		return _vector * _scalar;
	}

	template<typename T>
	inline Vector<3, T> operator/(T _scalar, const Vector<3, T>& _vector)
	{
		return Vector<3, T>(_scalar / _vector.x, _scalar / _vector.y, _scalar / _vector.z);
	}
}