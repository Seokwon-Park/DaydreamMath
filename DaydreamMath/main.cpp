#include <iostream>
#include <print>

#include "DaydreamMath.h"

using namespace Daydream;

int main()
{
	Vector3 scale(2.0f, 2.0f, 2.0f); // 2배 키움
	Vector3 rotEuler(0.0f, Math::DegreeToRadian(90.0f), 0.0f); // Y축 90도 회전
	Vector3 trans(10.0f, 0.0f, 0.0f); // X축으로 10만큼 이동

	Matrix4x4 S = Matrix4x4::CreateScale(scale);
	Matrix4x4 R = Matrix4x4::CreateRotation(Quaternion::CreateFromEuler(rotEuler));
	Matrix4x4 T = Matrix4x4::CreateTranslation(trans);

	Matrix4x4 world = S * R * T;

	Vector4 localPos(1.0f, 1.0f, 1.0f, 1.0f);

	Vector4 worldPos = localPos * world;
	std::print("World Pos: X={}, Y={}, Z={}, W={}", worldPos.x, worldPos.y, worldPos.z, worldPos.w);

	return 0;
}
