#pragma once

#include "Fwd.h"

namespace Daydream
{
	template<typename T>
	struct Matrix<4, 4, T>
	{
		union
		{
			T values[16];
			T mat[4][4];
		};

		Matrix()
		{
			for (UInt64 i = 0; i < 16; ++i) values[i] = static_cast<T>(0);
			mat[0][0] = static_cast<T>(1);
			mat[1][1] = static_cast<T>(1);
			mat[2][2] = static_cast<T>(1);
			mat[3][3] = static_cast<T>(1);
		}

		Matrix(std::initializer_list<T> _values)
		{
			assert(_values.size() <= 16 && "initializer list size must be less than or equal to 16");

			for (UInt64 i = 0; i < 16; ++i) values[i] = static_cast<T>(0);

			auto it = _values.begin();
			for (UInt64 i = 0; i < _values.size(); ++i, ++it)
			{
				values[i] = *it;
			}
		}

		T* operator[](UInt64 _row) { return mat[_row]; }
		const T* operator[](UInt64 _row) const { return mat[_row]; }

		static Matrix Identity()
		{
			return Matrix();
		}

		Matrix operator*(const Matrix& _other) const
		{
			Matrix<4, 4, Float32> result;

			SIMDRegister b0 = SIMD::LoadUnaligned(_other.mat[0]);
			SIMDRegister b1 = SIMD::LoadUnaligned(_other.mat[1]);
			SIMDRegister b2 = SIMD::LoadUnaligned(_other.mat[2]);
			SIMDRegister b3 = SIMD::LoadUnaligned(_other.mat[3]);

			// 2. 앞에 있는 행렬(*this, 즉 A)의 행을 하나씩 꺼내서 계산합니다.
			for (UInt64 i = 0; i < 4; ++i)
			{
				SIMDRegister rowA = SIMD::LoadUnaligned(mat[i]);

				// res = (A.x, A.x, A.x, A.x) * B의 0번 행
				SIMDRegister res = SIMD::Mul(SIMD::SplatX(rowA), b0);

				// res += (A.y, A.y, A.y, A.y) * B의 1번 행
				res = SIMD::MultiplyAdd(SIMD::SplatY(rowA), b1, res);

				// res += (A.z, A.z, A.z, A.z) * B의 2번 행
				res = SIMD::MultiplyAdd(SIMD::SplatZ(rowA), b2, res);

				// res += (A.w, A.w, A.w, A.w) * B의 3번 행
				res = SIMD::MultiplyAdd(SIMD::SplatW(rowA), b3, res);
				SIMD::StoreUnaligned(result.mat[i], res);
			}

			return result;
		}

		Matrix& operator*=(const Matrix& _other)
		{
			*this = (*this) * _other;
			return *this;
		}

		Vector<4, T> operator*(const Vector<4, T> _vec) const
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

		Matrix Transposed() const
		{
			Matrix result{};
			for (UInt64 r = 0; r < 4; ++r)
			{
				for (UInt64 c = 0; c < 4; ++c)
				{
					result.mat[r][c] = mat[c][r];
				}
			}
			return result;
		}

		//Matrix4x4 operator*(const Matrix4x4& _matrix) const;
		//Vector4 operator*(Vector3 _vector) const;
		//Vector4 operator*(Vector4 _vector) const;

		Vector<3, T> TransformPoint(const Vector<3, T>& _point) const
		{
			const T x = mat[0][0] * _point.x + mat[0][1] * _point.y + mat[0][2] * _point.z + mat[0][3];
			const T y = mat[1][0] * _point.x + mat[1][1] * _point.y + mat[1][2] * _point.z + mat[1][3];
			const T z = mat[2][0] * _point.x + mat[2][1] * _point.y + mat[2][2] * _point.z + mat[2][3];
			const T w = mat[3][0] * _point.x + mat[3][1] * _point.y + mat[3][2] * _point.z + mat[3][3];

			if (w == static_cast<T>(0) || w == static_cast<T>(1)) return Vector<3, T>(x, y, z);

			const T invW = static_cast<T>(1.0) / w;
			return Vector<3, T>(x * invW, y * invW, z * invW);
		}

		Vector<3, T> TransformVector(const Vector<3, T>& _vector) const
		{
			return Vector<3, T>(
				mat[0][0] * _vector.x + mat[0][1] * _vector.y + mat[0][2] * _vector.z,
				mat[1][0] * _vector.x + mat[1][1] * _vector.y + mat[1][2] * _vector.z,
				mat[2][0] * _vector.x + mat[2][1] * _vector.y + mat[2][2] * _vector.z);
		}

		static Matrix CreateTranslation(const Vector<3, T>& _translation);
		static Matrix CreateRotation(const Quat<T>& _quat);
		static Matrix CreateScale(const Vector<3, T>& _scale);
		static Matrix CreateLookToLH(const Vector<3, T>& _eye, const Vector<3, T>& _direction, const Vector<3, T>& _up);
		static Matrix CreatePerspectiveLH(T _fovy, T _aspect, T _near, T _far);
		static Matrix CreateOrthographicLH(T _width, T _height, T _near, T _far);
		static Matrix CreateTranspose(const Matrix<4, 4, T>& _m);
		static Matrix CreateInverse(const Matrix<4, 4, T>& m);
		static void Decompose(const Matrix<4, 4, T>& _world, Vector<3, T>& _outPosition, Quat<T>& _outRotation, Vector<3, T>& _outScale);
	};
}