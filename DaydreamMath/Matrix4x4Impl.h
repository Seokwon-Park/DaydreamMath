#pragma once

#include "Fwd.h"
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4SIMD.h"

//MatrixUtility
namespace Daydream
{
	template<typename T>
	inline Vector<4, T> operator*(const Vector<4, T>& _vec, const Matrix<4, 4, T>& _mat)
	{
		// 1. 행렬의 4개 행(Row)을 모두 로드
		SIMDRegister row0 = SIMD::LoadUnaligned(_mat[0]);
		SIMDRegister row1 = SIMD::LoadUnaligned(_mat[1]);
		SIMDRegister row2 = SIMD::LoadUnaligned(_mat[2]);
		SIMDRegister row3 = SIMD::LoadUnaligned(_mat[3]);

		// 2. 벡터의 각 성분(x, y, z, w)으로 행렬의 행을 스케일링해서 다 더하기
		SIMDRegister res = SIMD::Mul(SIMD::SplatX(_vec.reg), row0);
		res = SIMD::MultiplyAdd(SIMD::SplatY(_vec.reg), row1, res);
		res = SIMD::MultiplyAdd(SIMD::SplatZ(_vec.reg), row2, res);
		res = SIMD::MultiplyAdd(SIMD::SplatW(_vec.reg), row3, res);

		// 3. 결과를 Vector에 담아서 반환
		Vector<4, T> result;
		SIMD::StoreUnaligned(result.values, res);
		return result;
	}

	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::Inversed() const
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

		// 전체 행렬식(Determinant) 계산
		Vector<4, T> row0(inv0.x, inv1.x, inv2.x, inv3.x);
		Vector<4, T> dot0(m00 * row0.x, m01 * row0.y, m02 * row0.z, m03 * row0.w);
		T det = dot0.x + dot0.y + dot0.z + dot0.w;

		// 역행렬이 존재하지 않는 경우(det == 0) 방어 로직
		if (std::abs(det) <= static_cast<T>(1e-6))
		{
			return Matrix<4, 4, T>::Identity(); //실패시 단위행렬 리턴
		}

		//  각 원소를 행렬식으로 나누어 최종 역행렬을 완성
		T invDet = static_cast<T>(1.0) / det;
		Matrix<4, 4, T> result;

		result.mat[0][0] = inv0.x * invDet; result.mat[0][1] = inv0.y * invDet; result.mat[0][2] = inv0.z * invDet; result.mat[0][3] = inv0.w * invDet;
		result.mat[1][0] = inv1.x * invDet; result.mat[1][1] = inv1.y * invDet; result.mat[1][2] = inv1.z * invDet; result.mat[1][3] = inv1.w * invDet;
		result.mat[2][0] = inv2.x * invDet; result.mat[2][1] = inv2.y * invDet; result.mat[2][2] = inv2.z * invDet; result.mat[2][3] = inv2.w * invDet;
		result.mat[3][0] = inv3.x * invDet; result.mat[3][1] = inv3.y * invDet; result.mat[3][2] = inv3.z * invDet; result.mat[3][3] = inv3.w * invDet;

		return result;
	}

	template<typename T>
	inline Vector<3, T> Matrix<4, 4, T>::TransformPosition(const Vector<3, T>& _point, const Matrix<4, 4, T>& _mat)
	{
		T x = _point.x * _mat[0][0] + _point.y * _mat[1][0] + _point.z * _mat[2][0] + _mat[3][0];
		T y = _point.x * _mat[0][1] + _point.y * _mat[1][1] + _point.z * _mat[2][1] + _mat[3][1];
		T z = _point.x * _mat[0][2] + _point.y * _mat[1][2] + _point.z * _mat[2][2] + _mat[3][2];
		T w = _point.x * _mat[0][3] + _point.y * _mat[1][3] + _point.z * _mat[2][3] + _mat[3][3];

		if (w == static_cast<T>(1) || w == static_cast<T>(0)) return Vector<3, T>(x, y, z);

		T invW = static_cast<T>(1) / w;
		return Vector<3, T>(x * invW, y * invW, z * invW);
	}

	// [Direction * M] 방향 변환 (w = 0.0 으로 취급하여 이동값이 무시됨)
	template<typename T>
	inline Vector<3, T> Matrix<4, 4, T>::TransformDirection(const Vector<3, T>& _dir, const Matrix<4, 4, T>& _mat)
	{
		return Vector<3, T>(
			_dir.x * _mat[0][0] + _dir.y * _mat[1][0] + _dir.z * _mat[2][0],
			_dir.x * _mat[0][1] + _dir.y * _mat[1][1] + _dir.z * _mat[2][1],
			_dir.x * _mat[0][2] + _dir.y * _mat[1][2] + _dir.z * _mat[2][2]
		);
	}

	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::CreateTranslation(const Vector<3, T>& _translation)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();
		mat[3][0] = _translation.x; // 4번째 행으로 이동
		mat[3][1] = _translation.y;
		mat[3][2] = _translation.z;
		return mat;
	}


	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::CreateRotation(const Quat<T>& _quat)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();

		T xx = _quat.x * _quat.x;  T xy = _quat.x * _quat.y;  T xz = _quat.x * _quat.z;  T xw = _quat.x * _quat.w;
		T yy = _quat.y * _quat.y;  T yz = _quat.y * _quat.z;  T yw = _quat.y * _quat.w;
		T zz = _quat.z * _quat.z;  T zw = _quat.z * _quat.w;

		mat[0][0] = 1 - 2 * (yy + zz);
		mat[1][1] = 1 - 2 * (xx + zz);
		mat[2][2] = 1 - 2 * (xx + yy);

		mat[0][1] = 2 * (xy + zw); // 기존엔 - 였음
		mat[1][0] = 2 * (xy - zw); // 기존엔 + 였음

		mat[0][2] = 2 * (xz - yw); // 기존엔 + 였음
		mat[2][0] = 2 * (xz + yw); // 기존엔 - 였음

		mat[1][2] = 2 * (yz + xw); // 기존엔 - 였음
		mat[2][1] = 2 * (yz - xw); // 기존엔 + 였음

		return mat;
	}

	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::CreateScale(const Vector<3, T>& _scale)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();
		mat[0][0] = _scale.x;
		mat[1][1] = _scale.y;
		mat[2][2] = _scale.z;
		return mat;
	}

	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::CreateLookToLH(const Vector<3, T>& _eye, const Vector<3, T>& _direction, const Vector<3, T>& _up)
	{
		Vector<3, T> Look = _direction.Normalized();
		Vector<3, T> Right = Vector<3, T>::Cross(_up, Look).Normalized();
		Vector<3, T> Up = Vector<3, T>::Cross(Look, Right);

		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();

		// Row-Vector(v * M) 기준 
		mat[0][0] = Right.x; mat[0][1] = Up.x; mat[0][2] = Look.x;
		mat[1][0] = Right.y; mat[1][1] = Up.y; mat[1][2] = Look.y;
		mat[2][0] = Right.z; mat[2][1] = Up.z; mat[2][2] = Look.z;

		// 이동(Translation) 데이터
		mat[3][0] = -Vector<3, T>::Dot(Right, _eye);
		mat[3][1] = -Vector<3, T>::Dot(Up, _eye);
		mat[3][2] = -Vector<3, T>::Dot(Look, _eye);

		return mat;
	}

	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::CreatePerspectiveLH(T _fovy, T _aspect, T _near, T _far)
	{
		Matrix<4, 4, T> mat; 
		T tanHalfFovy = std::tan(_fovy * static_cast<T>(0.5));

		mat[0][0] = static_cast<T>(1) / (_aspect * tanHalfFovy);
		mat[1][1] = static_cast<T>(1) / tanHalfFovy;
		mat[2][2] = _far / (_far - _near);

		// 이 두 값의 위치를 맞바꿈!
		mat[2][3] = static_cast<T>(1);
		mat[3][2] = -(_far * _near) / (_far - _near);

		mat[3][3] = static_cast<T>(0);

		return mat;
	}

	template<typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::CreateOrthographicLH(T _left, T _right, T _bottom, T _top, T _near, T _far)
	{
		Matrix<4, 4, T> mat = Matrix<4, 4, T>::Identity();

		T width = _right - _left;
		T height = _top - _bottom;
		T depth = _far - _near;

		mat[0][0] = static_cast<T>(2.0) / width;
		mat[1][1] = static_cast<T>(2.0) / height;
		mat[2][2] = static_cast<T>(2.0) / depth;

		// 이동 데이터 4번째 행(Row)으로 이동
		mat[3][0] = -(_right + _left) / width;
		mat[3][1] = -(_top + _bottom) / height;
		mat[3][2] = -(_far + _near) / depth;
		mat[3][3] = static_cast<T>(1.0);

		return mat;
	}

	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::CreateOrthographicLH(T _width, T _height, T _near, T _far)
	{
		// 중심(0,0)을 기준으로 좌우/상하를 절반씩 찢어서 코어 함수에
		T halfW = _width / static_cast<T>(2.0);
		T halfH = _height / static_cast<T>(2.0);

		// 주의: LH-왼손좌표계 라면 Z축 방향 최적화가 RH와 약간 다를 수 있으나, 개념적으론 동일
		return CreateOrthographicLH(-halfW, halfW, -halfH, halfH, _near, _far);
	}

	template <typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::Transposed(const Matrix<4, 4, T>& _m)
	{
		Matrix<4, 4, T> result;
		for (int r = 0; r < 4; ++r)
			for (int c = 0; c < 4; ++c)
				result[r][c] = _m[c][r];
		return result;
	}
	template<typename T>
	inline Matrix<4, 4, T> Matrix<4, 4, T>::Inversed(const Matrix<4, 4, T>& m)
	{
		return m.Inversed();
	}
}


