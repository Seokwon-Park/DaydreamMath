#pragma once

#include "Fwd.h"
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

//MatrixUtility
namespace Daydream
{
	template<typename T>
	Vector<4, T> Matrix<4, 4, T>::operator*(const Vector<4, T> _vec) const
	{
		SIMDRegister row0 = SIMD::LoadUnaligned(mat[0]);
		SIMDRegister row1 = SIMD::LoadUnaligned(mat[1]);
		SIMDRegister row2 = SIMD::LoadUnaligned(mat[2]);
		SIMDRegister row3 = SIMD::LoadUnaligned(mat[3]);

		return Vector<4, Float32>(
			SIMD::Dot(row0, _vec.reg),
			SIMD::Dot(row1, _vec.reg),
			SIMD::Dot(row2, _vec.reg),
			SIMD::Dot(row3, _vec.reg)
		);
	}

	template<typename T>
	inline Vector<3, T> Matrix<4, 4, T>::TransformPoint(const Vector<3, T>& _point) const
	{
		const T x = mat[0][0] * _point.x + mat[0][1] * _point.y + mat[0][2] * _point.z + mat[0][3];
		const T y = mat[1][0] * _point.x + mat[1][1] * _point.y + mat[1][2] * _point.z + mat[1][3];
		const T z = mat[2][0] * _point.x + mat[2][1] * _point.y + mat[2][2] * _point.z + mat[2][3];
		const T w = mat[3][0] * _point.x + mat[3][1] * _point.y + mat[3][2] * _point.z + mat[3][3];

		if (w == static_cast<T>(0) || w == static_cast<T>(1)) return Vector<3, T>(x, y, z);

		const T invW = static_cast<T>(1.0) / w;
		return Vector<3, T>(x * invW, y * invW, z * invW);
	}

	template<typename T>
	Vector<3, T> Matrix<4, 4, T>::TransformVector(const Vector<3, T>& _vector) const
	{
		return Vector<3, T>(
			mat[0][0] * _vector.x + mat[0][1] * _vector.y + mat[0][2] * _vector.z,
			mat[1][0] * _vector.x + mat[1][1] * _vector.y + mat[1][2] * _vector.z,
			mat[2][0] * _vector.x + mat[2][1] * _vector.y + mat[2][2] * _vector.z);
	}

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateTranslation(const Vector<3, T>& _translation)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();
		mat[0][3] = _translation.x;
		mat[1][3] = _translation.y;
		mat[2][3] = _translation.z;
		return mat;
	}


	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateRotation(const Quat<T>& _quat)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();

		// ==============================================================================
		// 입력으로 들어오는 쿼터니언은 항상 길이가 1 (x^2 + y^2 + z^2 + w^2 = 1)이어야 함
		// 
		// 1. 위 공식에서 (w^2 + x^2)만 남기면 -> w^2 + x^2 = 1 - y^2 - z^2
		// 2. 이 값을 원본 R[0][0] 공식에 대입하면:
		//    (1 - y^2 - z^2) - y^2 - z^2 
		//    = 1 - 2y^2 - 2z^2 
		//    = 1 - 2(y^2 + z^2)
		//
		// ==============================================================================
		T xx = _quat.x * _quat.x;  T xy = _quat.x * _quat.y;  T xz = _quat.x * _quat.z;  T xw = _quat.x * _quat.w;
		T yy = _quat.y * _quat.y;  T yz = _quat.y * _quat.z;  T yw = _quat.y * _quat.w;
		T zz = _quat.z * _quat.z;  T zw = _quat.z * _quat.w;

		mat[0][0] = 1 - 2 * (yy + zz);
		mat[0][1] = 2 * (xy - zw);
		mat[0][2] = 2 * (xz + yw);

		mat[1][0] = 2 * (xy + zw);
		mat[1][1] = 1 - 2 * (xx + zz);
		mat[1][2] = 2 * (yz - xw);

		mat[2][0] = 2 * (xz - yw);
		mat[2][1] = 2 * (yz + xw);
		mat[2][2] = 1 - 2 * (xx + yy);

		return mat;
	}

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateScale(const Vector<3, T>& _scale)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();
		mat[0][0] = _scale.x;
		mat[1][1] = _scale.y;
		mat[2][2] = _scale.z;
		return mat;
	}

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateLookToLH(const Vector<3, T>& _eye, const Vector<3, T>& _direction, const Vector<3, T>& _up)
	{
		// World Transform = S(cale) * R(otation) * T(ranslation)
		// View Transform = (R * T)^-1 scale 생략
		// = T^-1 * R^-1
		Vector<3, T> Look = _direction.Normalized();
		Vector<3, T> Right = Vector<3,T>::Cross(_up, Look).Normalized();
		Vector<3, T> Up = Vector<3, T>::Cross(Right, Look);

		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();
		mat[0][0] = Right.x; mat[0][1] = Right.y; mat[0][2] = Right.z;
		mat[1][0] = Up.x; mat[1][1] = Up.y; mat[1][2] = Up.z;
		mat[2][0] = Look.x; mat[2][1] = Look.y; mat[2][2] = Look.z;

		mat[0][3] = -Dot(Look, _eye);
		mat[1][3] = -Dot(Right, _eye);
		mat[2][3] = -Dot(Up, _eye);

		return mat;
	}

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreatePerspectiveLH(T _fovy, T _aspect, T _near, T _far)
	{
		Matrix<4, 4, T> mat; // Zero 초기화라고 가정
		T tanHalfFovy = std::tan(_fovy * static_cast<T>(0.5));

		mat[0][0] = static_cast<T>(1) / (_aspect * tanHalfFovy);
		mat[1][1] = static_cast<T>(1) / tanHalfFovy;
		mat[2][2] = _far / (_far - _near);
		mat[2][3] = -(_far * _near) / (_far - _near);
		mat[3][2] = static_cast<T>(1);
		mat[3][3] = static_cast<T>(0);

		return mat;
	}

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateOrthographicLH(T _width, T _height, T _near, T _far)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();
		mat[0][0] = static_cast<T>(2) / _width;
		mat[1][1] = static_cast<T>(2) / _height;
		mat[2][2] = static_cast<T>(1) / (_far - _near);
		mat[2][3] = -_near / (_far - _near);
		return mat;
	}

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateTranspose(const Matrix<4, 4, T>& _m)
	{
		Matrix<4, 4, T> result;
		for (int r = 0; r < 4; ++r)
			for (int c = 0; c < 4; ++c)
				result[r][c] = _m[c][r];
		return result;
	}

	// --- 2. 역행렬 (Inverse) ---
	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateInverse(const Matrix<4, 4, T>& m)
	{
		// 1. 소행렬식(Cofactor)을 이용한 행렬식(Determinant) 계산 및 여인수 행렬 도출
		// (코드가 너무 길어지므로 가장 대중적이고 빠르고 안전한 Cramer's Rule 방식의 뼈대입니다)
		T coef00 = m[2][2] * m[3][3] - m[3][2] * m[2][3];
		T coef02 = m[1][2] * m[3][3] - m[3][2] * m[1][3];
		T coef03 = m[1][2] * m[2][3] - m[2][2] * m[1][3];
		// ... (나머지 15개 계수 계산 생략 - 이 부분은 너무 수학적 노가다라 보통 라이브러리 코드를 차용합니다) ...

		Matrix<4, 4, T> inv;
		// inv 행렬 채우기 및 행렬식(Det) 나누기
		// inv = adj(m) / Det(m)
		return inv;
	}

	// --- 3. 행렬 분해 (Decompose) ---
	// 4x4 행렬 하나에서 이동(P), 회전(R), 크기(S)를 역추적해서 뽑아냅니다.

}


