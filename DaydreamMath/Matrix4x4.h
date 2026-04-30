#pragma once

#include "Fwd.h"
#include "SIMD.h"

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

		explicit Matrix(const T* _dataPtr)
		{
			std::memcpy(values, _dataPtr, sizeof(T) * 16);
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

		static Matrix Identity()
		{
			return Matrix();
		}

		T* operator[](UInt64 _row) { return mat[_row]; }
		const T* operator[](UInt64 _row) const { return mat[_row]; }

		//이거 좀 지리는듯 ㄷ;;
		Matrix operator*(const Matrix& _other) const
		{
			Matrix<4, 4, T> result;

			SIMDRegister b0 = SIMD::LoadUnaligned(_other.mat[0]);
			SIMDRegister b1 = SIMD::LoadUnaligned(_other.mat[1]);
			SIMDRegister b2 = SIMD::LoadUnaligned(_other.mat[2]);
			SIMDRegister b3 = SIMD::LoadUnaligned(_other.mat[3]);

			// 2. 앞에 있는 행렬(*this, 즉 A)의 행을 하나씩 꺼내서 계산
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

		void Transpose()
		{
			*this = Transposed();
		}

		Matrix Inversed() const;
		void Inverse()
		{
			*this = Inversed();
		}


		//Matrix4x4 operator*(const Matrix4x4& _matrix) const;
		//Vector4 operator*(Vector3 _vector) const;
		//Vector4 operator*(Vector4 _vector) const;

		static Vector<3, T> TransformPosition(const Vector<3, T>& _point, const Matrix<4, 4, T>& _mat);
		static Vector<3, T> TransformDirection(const Vector<3, T>& _point, const Matrix<4, 4, T>& _mat);


		static Matrix CreateTranslation(const Vector<3, T>& _translation);
		static Matrix CreateRotation(const Quat<T>& _quat);
		static Matrix CreateScale(const Vector<3, T>& _scale);
		static Matrix CreateLookToLH(const Vector<3, T>& _eye, const Vector<3, T>& _direction, const Vector<3, T>& _up);
		static Matrix CreatePerspectiveLH(T _fovy, T _aspect, T _near, T _far);
		static Matrix CreateOrthographicLH(T _left, T _right, T _bottom, T _top, T _near, T _far);
		static Matrix CreateOrthographicLH(T _width, T _height, T _near, T _far);
		static Matrix Transposed(const Matrix<4, 4, T>& _m);
		static Matrix Inversed(const Matrix<4, 4, T>& m);
	};
}