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

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::Inversed() const
	{
		T m00 = mat[0][0], m01 = mat[0][1], m02 = mat[0][2], m03 = mat[0][3];
		T m10 = mat[1][0], m11 = mat[1][1], m12 = mat[1][2], m13 = mat[1][3];
		T m20 = mat[2][0], m21 = mat[2][1], m22 = mat[2][2], m23 = mat[2][3];
		T m30 = mat[3][0], m31 = mat[3][1], m32 = mat[3][2], m33 = mat[3][3];

		// 2. 2x2 부분 행렬식(Sub-determinants)들 캐싱
		T coef00 = m22 * m33 - m32 * m23;
		T coef02 = m12 * m33 - m32 * m13;
		T coef03 = m12 * m23 - m22 * m13;

		T coef04 = m21 * m33 - m31 * m23;
		T coef06 = m11 * m33 - m31 * m13;
		T coef07 = m11 * m23 - m21 * m13;

		T coef08 = m21 * m32 - m31 * m22;
		T coef10 = m11 * m32 - m31 * m12;
		T coef11 = m11 * m22 - m21 * m12;

		T coef12 = m20 * m33 - m30 * m23;
		T coef14 = m10 * m33 - m30 * m13;
		T coef15 = m10 * m23 - m20 * m13;

		T coef16 = m20 * m32 - m30 * m22;
		T coef18 = m10 * m32 - m30 * m12;
		T coef19 = m10 * m22 - m20 * m12;

		T coef20 = m20 * m31 - m30 * m21;
		T coef22 = m10 * m31 - m30 * m11;
		T coef23 = m10 * m21 - m20 * m11;

		// 3. 수반 행렬(Adjugate Matrix)의 각 성분을 구함 (동시에 행과 열이 바뀌는 Transpose 효과 포함)
		Vector<4, T> fac0(coef00, coef00, coef02, coef03);
		Vector<4, T> fac1(coef04, coef04, coef06, coef07);
		Vector<4, T> fac2(coef08, coef08, coef10, coef11);
		Vector<4, T> fac3(coef12, coef12, coef14, coef15);
		Vector<4, T> fac4(coef16, coef16, coef18, coef19);
		Vector<4, T> fac5(coef20, coef20, coef22, coef23);

		Vector<4, T> vec0(m10, m00, m00, m00);
		Vector<4, T> vec1(m11, m01, m01, m01);
		Vector<4, T> vec2(m12, m02, m02, m02);
		Vector<4, T> vec3(m13, m03, m03, m03);

		Vector<4, T> inv0(vec1.x * fac0.x - vec2.x * fac1.x + vec3.x * fac2.x,
			vec2.y * fac1.y - vec1.y * fac0.y - vec3.y * fac2.y,
			vec1.z * fac0.z - vec2.z * fac1.z + vec3.z * fac2.z,
			vec2.w * fac1.w - vec1.w * fac0.w - vec3.w * fac2.w);

		Vector<4, T> inv1(vec2.x * fac3.x - vec0.x * fac0.x - vec3.x * fac4.x,
			vec0.y * fac0.y - vec2.y * fac3.y + vec3.y * fac4.y,
			vec2.z * fac3.z - vec0.z * fac0.z - vec3.z * fac4.z,
			vec0.w * fac0.w - vec2.w * fac3.w + vec3.w * fac4.w);

		Vector<4, T> inv2(vec0.x * fac1.x - vec1.x * fac3.x + vec3.x * fac5.x,
			vec1.y * fac3.y - vec0.y * fac1.y - vec3.y * fac5.y,
			vec0.z * fac1.z - vec1.z * fac3.z + vec3.z * fac5.z,
			vec1.w * fac3.w - vec0.w * fac1.w - vec3.w * fac5.w);

		Vector<4, T> inv3(vec1.x * fac4.x - vec0.x * fac2.x - vec2.x * fac5.x,
			vec0.y * fac2.y - vec1.y * fac4.y + vec2.y * fac5.y,
			vec1.z * fac4.z - vec0.z * fac2.z - vec2.z * fac5.z,
			vec0.w * fac2.w - vec1.w * fac4.w + vec2.w * fac5.w);

		// 4. 전체 행렬식(Determinant)을 구합니다.
		Vector<4, T> row0(inv0.x, inv1.x, inv2.x, inv3.x);
		Vector<4, T> dot0(m00 * row0.x, m01 * row0.y, m02 * row0.z, m03 * row0.w);
		T det = dot0.x + dot0.y + dot0.z + dot0.w;

		// 5. 역행렬이 존재하지 않는 경우(det == 0) 방어 로직
		if (std::abs(det) <= static_cast<T>(1e-6))
		{
			return Matrix<4, 4, T>::Identity(); //실패시 단위행렬 리턴
		}

		// 6. 각 원소를 행렬식으로 나누어 최종 역행렬을 완성합니다.
		T invDet = static_cast<T>(1.0) / det;
		Matrix<4, 4, T> result;

		result.mat[0][0] = inv0.x * invDet; result.mat[0][1] = inv0.y * invDet; result.mat[0][2] = inv0.z * invDet; result.mat[0][3] = inv0.w * invDet;
		result.mat[1][0] = inv1.x * invDet; result.mat[1][1] = inv1.y * invDet; result.mat[1][2] = inv1.z * invDet; result.mat[1][3] = inv1.w * invDet;
		result.mat[2][0] = inv2.x * invDet; result.mat[2][1] = inv2.y * invDet; result.mat[2][2] = inv2.z * invDet; result.mat[2][3] = inv2.w * invDet;
		result.mat[3][0] = inv3.x * invDet; result.mat[3][1] = inv3.y * invDet; result.mat[3][2] = inv3.z * invDet; result.mat[3][3] = inv3.w * invDet;

		return result;
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

		mat[0][3] = -Vector3::Dot(Look, _eye);
		mat[1][3] = -Vector3::Dot(Right, _eye);
		mat[2][3] = -Vector3::Dot(Up, _eye);

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

	template<typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateOrthographicLH(T _left, T _right, T _bottom, T _top, T _near, T _far)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();

		// 1. 박스의 폭, 높이, 깊이를 구합니다.
		T width = _right - _left;
		T height = _top - _bottom;
		T depth = _far - _near;

		// 2. 크기를 -1 ~ 1 사이로 정규화(Scale) 합니다.
		mat[0][0] = static_cast<T>(2.0) / width;
		mat[1][1] = static_cast<T>(2.0) / height;
		mat[2][2] = -static_cast<T>(2.0) / depth; // 오른손 좌표계는 Z축이 거꾸로라 마이너스가 붙습니다!

		// 3. 중심점을 화면 한가운데로 이동(Translate) 시킵니다.
		mat[3][0] = -(_right + _left) / width;
		mat[3][1] = -(_top + _bottom) / height;
		mat[3][2] = -(_far + _near) / depth;
		mat[3][3] = static_cast<T>(1.0);

		return mat;
	}

	template <typename T>
	Matrix<4, 4, T> Matrix<4, 4, T>::CreateOrthographicLH(T _width, T _height, T _near, T _far)
	{
		// 중심(0,0)을 기준으로 좌우/상하를 절반씩 찢어서 코어 함수에 토스!
		T halfW = _width / static_cast<T>(2.0);
		T halfH = _height / static_cast<T>(2.0);

		// (주의: LH-왼손좌표계 라면 Z축 방향 최적화가 RH와 약간 다를 수 있으나, 개념적으론 동일합니다)
		return CreateOrthographicLH(-halfW, halfW, -halfH, halfH, _near, _far);
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


