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

		Vector(T _x, T _y, T _z)
			: x(_x), y(_y), z(_z)
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

		T& operator[](int _index) { return values[_index]; }
		const T& operator[](int _index) const { return values[_index]; }

		Vector operator-() const { return Vector(-x, -y, -z); }
		Vector operator+(const Vector& _other) const { return Vector(x + _other.x, y + _other.y, z + _other.z); }
		Vector operator-(const Vector& _other) const { return Vector(x - _other.x, y - _other.y, z - _other.z); }
		Vector operator*(const Vector& _other) const { return Vector(x * _other.x, y * _other.y, z * _other.z); }
		Vector operator/(const Vector& _other) const { return Vector(x / _other.x, y / _other.y, z / _other.z); }

		// 5. 스칼라 사칙연산
		Vector operator+(T _scalar) const { return Vector(x + _scalar, y + _scalar, z + _scalar); }
		Vector operator-(T _scalar) const { return Vector(x - _scalar, y - _scalar, z - _scalar); }
		Vector operator*(T _scalar) const { return Vector(x * _scalar, y * _scalar, z * _scalar); }
		Vector operator/(T _scalar) const
		{
			T inv = static_cast<T>(1.0) / _scalar;
			return Vector(x * inv, y * inv, z * inv);
		}

		Vector& operator+=(const Vector& _other) { x += _other.x; y += _other.y; z += _other.z; return *this; }
		Vector& operator-=(const Vector& _other) { x -= _other.x; y -= _other.y; z -= _other.z; return *this; }
		Vector& operator*=(T _scalar) { x *= _scalar; y *= _scalar; z *= _scalar; return *this; }

		bool operator==(const Vector& _other) const { return x == _other.x && y == _other.y && z == _other.z; }
		bool operator!=(const Vector& _other) const { return !(*this == _other); }
	};
}