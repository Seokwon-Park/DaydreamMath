#pragma once

#include "Quaternion.h"
#include "Matrix4x4.h"

namespace Daydream
{
	template <typename T>
	Quat<T> Quat<T>::Slerp(const Quat<T>& _quatA, const Quat<T>& _quatB, T _t)
    {
        Quat<T> q3 = _quatB;
        T cosTheta = Dot(_quatA, _quatB);

        // 내적값이 음수면 지구 반대편으로 돌아가는 꼴입니다.
        // 목표 쿼터니언의 부호를 싹 다 뒤집어서(반전) 가까운 길로 유도합니다!
        if (cosTheta < static_cast<T>(0.0))
        {
            cosTheta = -cosTheta;
            q3.x = -q3.x; q3.y = -q3.y; q3.z = -q3.z; q3.w = -q3.w;
        }

        // 두 회전값이 거의 똑같을 때(cosTheta가 1에 가까울 때) acos나 sin 연산을 하면 
        // 0으로 나누기가 터져서(NaN) 게임이 튕깁니다. 이때는 그냥 1차원 선형 보간(Lerp)으로 퉁칩니다!
        if (cosTheta > static_cast<T>(0.9999))
        {
            Quat<T> result(
                _quatA.x + _t * (q3.x - _quatA.x),
                _quatA.y + _t * (q3.y - _quatA.y),
                _quatA.z + _t * (q3.z - _quatA.z),
                _quatA.w + _t * (q3.w - _quatA.w)
            );

            // Lerp를 거치면 길이가 1이 아니게 되므로 무조건 다시 정규화(Normalize) 해줍니다.
            T invLen = static_cast<T>(1.0) / std::sqrt(Dot(result, result));
            result.x *= invLen; result.y *= invLen; result.z *= invLen; result.w *= invLen;
            return result;
        }

        T angle = std::acos(cosTheta);
        T sinTheta = std::sin(angle);
        T invSinTheta = static_cast<T>(1.0) / sinTheta;

        // 두 회전값에 각각 줄 가중치(Weight) 계산
        T weight1 = std::sin((static_cast<T>(1.0) - _t) * angle) * invSinTheta;
        T weight2 = std::sin(_t * angle) * invSinTheta;

        return Quat<T>(
            _quatA.x * weight1 + q3.x * weight2,
            _quatA.y * weight1 + q3.y * weight2,
            _quatA.z * weight1 + q3.z * weight2,
            _quatA.w * weight1 + q3.w * weight2
        );
    }

    //https://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/
    // 아몰랑 그냥 식만 베껴
    template <typename T>
    Quat<T> Quat<T>::CreateFromMatrix(const Matrix<4, 4, T>& _matrix)
    {
        T trace = _matrix.mat[0][0] + _matrix.mat[1][1] + _matrix.mat[2][2];

        if (trace > static_cast<T>(0.0))
        {
            // s = 4 * qw
            T s = std::sqrt(trace + static_cast<T>(1.0)) * static_cast<T>(2.0);
            T invS = static_cast<T>(1.0) / s;

            return Quat<T>(
                (_matrix.mat[2][1] - _matrix.mat[1][2]) * invS,
                (_matrix.mat[0][2] - _matrix.mat[2][0]) * invS,
                (_matrix.mat[1][0] - _matrix.mat[0][1]) * invS,
                static_cast<T>(0.25) * s
            );
        }
        else
        {
            if (_matrix.mat[0][0] > _matrix.mat[1][1] && _matrix.mat[0][0] > _matrix.mat[2][2])
            {
                // s = 4 * qx
                T s = std::sqrt(static_cast<T>(1.0) + _matrix.mat[0][0] - _matrix.mat[1][1] - _matrix.mat[2][2]) * static_cast<T>(2.0);
                T invS = static_cast<T>(1.0) / s;

                return Quat<T>(
                    static_cast<T>(0.25) * s,
                    (_matrix.mat[0][1] + _matrix.mat[1][0]) * invS,
                    (_matrix.mat[0][2] + _matrix.mat[2][0]) * invS,
                    (_matrix.mat[2][1] - _matrix.mat[1][2]) * invS
                );
            }
            else if (_matrix.mat[1][1] > _matrix.mat[2][2])
            {
                // s = 4 * qy
                T s = std::sqrt(static_cast<T>(1.0) + _matrix.mat[1][1] - _matrix.mat[0][0] - _matrix.mat[2][2]) * static_cast<T>(2.0);
                T invS = static_cast<T>(1.0) / s;

                return Quat<T>(
                    (_matrix.mat[0][1] + _matrix.mat[1][0]) * invS,
                    static_cast<T>(0.25) * s,
                    (_matrix.mat[1][2] + _matrix.mat[2][1]) * invS,
                    (_matrix.mat[0][2] - _matrix.mat[2][0]) * invS
                );
            }
            else
            {
                // s = 4 * qz
                T s = std::sqrt(static_cast<T>(1.0) + _matrix.mat[2][2] - _matrix.mat[0][0] - _matrix.mat[1][1]) * static_cast<T>(2.0);
                T invS = static_cast<T>(1.0) / s;

                return Quat<T>(
                    (_matrix.mat[0][2] + _matrix.mat[2][0]) * invS,
                    (_matrix.mat[1][2] + _matrix.mat[2][1]) * invS,
                    static_cast<T>(0.25) * s,
                    (_matrix.mat[1][0] - _matrix.mat[0][1]) * invS
                );
            }
        }
    }
}