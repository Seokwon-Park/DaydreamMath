#include <iostream>
#include <print>

#include "DaydreamMath.h"

using namespace Daydream;

int main()
{
	// 1. 테스트용 임의의 값 설정
	Vector3 scale(2.0f, 2.0f, 2.0f); // 2배 키움
	Vector3 rotEuler(0.0f, Math::DegreeToRadian(90.0f), 0.0f); // Y축 90도 회전
	Vector3 trans(10.0f, 0.0f, 0.0f); // X축으로 10만큼 이동

	// 2. 완벽한 Row-Vector 기준의 월드 행렬 조립 (S * R * T)
	Matrix4x4 S = Matrix4x4::CreateScale(scale);
	Matrix4x4 R = Matrix4x4::CreateRotation(Quaternion::CreateFromEuler(rotEuler));
	Matrix4x4 T = Matrix4x4::CreateTranslation(trans);

	Matrix4x4 world = S * R * T;

	// 3. 로컬 정점 (우측 상단 모서리) 생성 (w = 1.0 -> 점)
	Vector4 localPos(1.0f, 1.0f, 1.0f, 1.0f);

	// 4. 대망의 Vector * Matrix 변환!
	Vector4 worldPos = localPos * world;

	// ==========================================
	// 1) S 적용: (1, 1, 1) -> (2, 2, 2)
	// 2) R 적용: Y축으로 90도 돌림. Z축이 X축 방향으로, X축이 -Z축 방향으로 감 -> (2, 2, -2)
	// 3) T 적용: X축으로 10 더함 -> 최종 좌표 (12, 2, -2)
	// ==========================================

	// 콘솔에 출력 (만약 X=12, Y=2, Z=-2, W=1 이 나오면 엔진 수학 완벽 졸업입니다!)
	std::print("World Pos: X={}, Y={}, Z={}, W={}", worldPos.x, worldPos.y, worldPos.z, worldPos.w);

	return 0;
}
