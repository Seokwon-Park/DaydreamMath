#pragma once

#include "Quaternion.h"
#include "Matrix4x4.h"

namespace Daydream
{
    template<typename T>
    inline Quat<T> Quat<T>::FromAxisAngle(const Vector<3, T>& _axis, T _radian)
    {
        const T axisLength = static_cast<T>(std::sqrt(static_cast<Float64>(_axis.x * _axis.x + _axis.y * _axis.y + _axis.z * _axis.z)));
        if (axisLength == static_cast<T>(0)) return Identity();

        const T half = _radian * static_cast<T>(0.5);
        const T sinHalf = static_cast<T>(std::sin(static_cast<Float64>(half)));
        const T cosHalf = static_cast<T>(std::cos(static_cast<Float64>(half)));
        const T invAxisLength = static_cast<T>(1.0) / axisLength;

        return Quat(
            _axis.x * invAxisLength * sinHalf,
            _axis.y * invAxisLength * sinHalf,
            _axis.z * invAxisLength * sinHalf,
            cosHalf);
    }

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
    template <typename T>
    Quat<T> Quat<T>::CreateFromAxis(const Vector<3, T>& _xAxis, const Vector<3, T>& _yAxis, const Vector<3, T>& _zAxis)
    {
        // 1. 빈 단위 행렬(포장지)을 하나 가져옵니다.
        Matrix<4, 4, T> tempMat = Matrix<4, 4, T>::Identity();

        // 2. 포장지의 각 줄(Row)에 3개의 축을 그대로 꽂아 넣습니다!
        tempMat.mat[0][0] = _xAxis.x; tempMat.mat[0][1] = _xAxis.y; tempMat.mat[0][2] = _xAxis.z;
        tempMat.mat[1][0] = _yAxis.x; tempMat.mat[1][1] = _yAxis.y; tempMat.mat[1][2] = _yAxis.z;
        tempMat.mat[2][0] = _zAxis.x; tempMat.mat[2][1] = _zAxis.y; tempMat.mat[2][2] = _zAxis.z;

        // 3. 방금 유로클리디안 스페이스에서 복붙해오신 그 완벽한 함수에 던집니다!
        return CreateFromMatrix(tempMat);
    }
    template<typename T>
    Quat<T> Quat<T>::CreateFromEuler(const Vector<3, T>& _euler)
    {
        // 1. 각도를 반으로 나눔
        T halfPitch = _euler.x * static_cast<T>(0.5);
        T halfYaw = _euler.y * static_cast<T>(0.5);
        T halfRoll = _euler.z * static_cast<T>(0.5);

        // 2. sin, cos 값을 미리 구함
        T sp = std::sin(halfPitch);
        T cp = std::cos(halfPitch);
        T sy = std::sin(halfYaw);
        T cy = std::cos(halfYaw);
        T sr = std::sin(halfRoll);
        T cr = std::cos(halfRoll);

        // 3. Yaw -> Pitch -> Roll 순서로 곱해진 결과값
        return Quat<T>(
            sp * cy * cr - cp * sy * sr, // X
            cp * sy * cr + sp * cy * sr, // Y
            cp * cy * sr - sp * sy * cr, // Z
            cp * cy * cr + sp * sy * sr  // W
        );
    }
    template<typename T>
    inline Vector<3, T> Quat<T>::ToEuler() const
    {
        Vector<3, T> euler;

        // 1. X축 회전 (Pitch) 계산 및 짐벌락 방어!
        T sinp = static_cast<T>(2.0) * (w * x - y * z);

        // sinp 값이 1.0을 넘어가면 (부동소수점 오차) std::asin 이 NaN을 뱉고 게임이 터집니다!
        if (std::abs(sinp) >= static_cast<T>(1.0))
        {
            // 90도 (PI / 2) 로 강제 고정하되, 부호는 원래 sinp의 부호를 따라갑니다.
            euler.x = std::copysign(static_cast<T>(3.14159265358979323846 / 2.0), sinp);
        }
        else
        {
            euler.x = std::asin(sinp);
        }

        // 2. Y축 회전 (Yaw)
        T siny_cosp = static_cast<T>(2.0) * (w * y + z * x);
        T cosy_cosp = static_cast<T>(1.0) - static_cast<T>(2.0) * (x * x + y * y);
        euler.y = std::atan2(siny_cosp, cosy_cosp);

        // 3. Z축 회전 (Roll)
        T sinr_cosp = static_cast<T>(2.0) * (w * z + x * y);
        T cosr_cosp = static_cast<T>(1.0) - static_cast<T>(2.0) * (x * x + z * z);
        euler.z = std::atan2(sinr_cosp, cosr_cosp);

        return euler;
    }
}