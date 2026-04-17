#pragma once

#include "Fwd.h"

namespace Daydream
{
	template <typename T>
	struct Quat
	{
		union
		{
			struct
			{
				T x, y, z, w;
			};
			T values[4];
		};

		Quat()
			: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(1))
		{
		}

		Quat(T _x, T _y, T _z, T _w)
			: x(_x), y(_y), z(_z), w(_w)
		{
		}


		explicit Quat(Vector3 _eulerAngles)
		{
			// 1. 각도를 반으로 나눔
			T halfPitch = _eulerAngles.x * static_cast<T>(0.5);
			T halfYaw = _eulerAngles.y * static_cast<T>(0.5);
			T halfRoll = _eulerAngles.z * static_cast<T>(0.5);

			// 2. sin, cos 값을 미리 구함
			T sp = std::sin(halfPitch);
			T cp = std::cos(halfPitch);
			T sy = std::sin(halfYaw);
			T cy = std::cos(halfYaw);
			T sr = std::sin(halfRoll);
			T cr = std::cos(halfRoll);

			x = sp * cy * cr - cp * sy * sr; // X
			y = cp * sy * cr + sp * cy * sr; // Y
			z = cp * cy * sr - sp * sy * cr; // Z
			w = cp * cy * cr + sp * sy * sr;  // W
		}

		Quat(std::initializer_list<T> _values)
			: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(1))
		{
			assert(_values.size() <= 4 && "initializer list size must be less than or equal to 4");

			auto it = _values.begin();
			if (it != _values.end()) x = *it++;
			if (it != _values.end()) y = *it++;
			if (it != _values.end()) z = *it++;
			if (it != _values.end()) w = *it++;
		}

		Quat& operator=(std::initializer_list<T> _values)
		{
			assert(_values.size() <= 4 && "initializer list size must be less than or equal to 4");

			x = static_cast<T>(0);
			y = static_cast<T>(0);
			z = static_cast<T>(0);
			w = static_cast<T>(1);

			auto it = _values.begin();
			if (it != _values.end()) x = *it++;
			if (it != _values.end()) y = *it++;
			if (it != _values.end()) z = *it++;
			if (it != _values.end()) w = *it++;
			return *this;
		}

		T& operator[](UInt64 _index) { return values[_index]; }
		const T& operator[](UInt64 _index) const { return values[_index]; }

		// 어떤 회전도 하지 않은 상태
		static Quat Identity()
		{
			return Quat();
		}

		// 두 회전이 얼마나 비슷한가
		static T Dot(const Quat& _a, const Quat& _b)
		{
			return _a.x * _b.x + _a.y * _b.y + _a.z * _b.z + _a.w * _b.w;
		}

		static Quat FromAxisAngle(const Vector<3, T>& _axis, T _radian);

		static Quat Slerp(const Quat<T>& _q1, const Quat<T>& _q2, T _t);
		static Quat CreateFromMatrix(const Matrix<4, 4, T>& _matrix);
		static Quat CreateFromAxis(const Vector<3, T>& _xAxis, const Vector<3, T>& _yAxis, const Vector<3, T>& _zAxis);

		static Quat CreateFromEuler(const Vector<3, T>& _eulerAngles);
		inline Vector<3, T> ToEuler() const;


		Quat operator+(const Quat& _other) const { return Quat(x + _other.x, y + _other.y, z + _other.z, w + _other.w); }
		Quat operator-(const Quat& _other) const { return Quat(x - _other.x, y - _other.y, z - _other.z, w - _other.w); }
		Quat operator*(const Quat& _other) const
		{
			return Quat(
				w * _other.x + x * _other.w + y * _other.z - z * _other.y,
				w * _other.y - x * _other.z + y * _other.w + z * _other.x,
				w * _other.z + x * _other.y - y * _other.x + z * _other.w,
				w * _other.w - x * _other.x - y * _other.y - z * _other.z);
		}

		Quat operator*(T _scalar) const { return Quat(x * _scalar, y * _scalar, z * _scalar, w * _scalar); }
		Quat operator/(T _scalar) const
		{
			const T inv = static_cast<T>(1.0) / _scalar;
			return *this * inv;
		}

		Quat& operator+=(const Quat& _other) { x += _other.x; y += _other.y; z += _other.z; w += _other.w; return *this; }
		Quat& operator-=(const Quat& _other) { x -= _other.x; y -= _other.y; z -= _other.z; w -= _other.w; return *this; }
		Quat& operator*=(const Quat& _other) { *this = *this * _other; return *this; }

		Quat& operator*=(T _scalar) { x *= _scalar; y *= _scalar; z *= _scalar; w *= _scalar; return *this; }
		Quat& operator/=(T _scalar) { *this = *this / _scalar; return *this; }

		bool operator==(const Quat& _other) const { return x == _other.x && y == _other.y && z == _other.z && w == _other.w; }
		bool operator!=(const Quat& _other) const { return !(*this == _other); }

		T LengthSq() const
		{
			return x * x + y * y + z * z + w * w;
		}

		T Length() const
		{
			return static_cast<T>(std::sqrt(static_cast<Float64>(LengthSq())));
		}

		Quat Conjugate() const
		{
			return Quat(-x, -y, -z, w);
		}

		Quat Inverse() const
		{
			const T lengthSq = LengthSq();
			if (lengthSq == static_cast<T>(0)) return Identity();
			return Conjugate() / lengthSq;
		}

		Quat Normalized() const
		{
			const T length = Length();
			if (length == static_cast<T>(0)) return Identity();
			return *this / length;
		}

		void Normalize()
		{
			*this = Normalized();
		}
	};

	template <typename T>
	inline Quat<T> operator*(T _scalar, const Quat<T>& _quat)
	{
		return _quat * _scalar;
	}
}