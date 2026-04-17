#pragma once

#include "Fwd.h"
#include "Vector3.h"

namespace Daydream
{
	template <typename T>
	inline Vector<3, T> operator*(const Vector<3, T>& _v, const Matrix<4, 4, T>& _m) = delete;

	template <typename T>
	inline Vector<3, T> operator*(const Matrix<4, 4, T>& _m, const Vector<3, T>& _v) = delete;

	template <typename T>
	Vector<3, T> Vector<3, T>::Rotate(const Quat<T>& _quat, const Vector<3, T> _vector)
	{
		Vector<3, T> qVec(_quat.x, _quat.y, _quat.z);
		Vector<3, T> t = Cross(qVec, _v) * static_cast<T>(2.0);
		return _vector + (t * _quat.w) + Cross(qVec, t);
	}
}
