# DaydreamMath

DaydreamMath는 C++ 기반 수학 라이브러리의 **기본 골격(scaffold)** 입니다.

## 현재 포함된 구성

- `MathCommon.h`
  - 공통 타입 별칭 (`Int32`, `Float32` 등)
  - 수학 상수 (`kPi`, `kEpsilon`)
  - 유틸 함수 (`NearlyEqual`, `Clamp`, `Lerp`)
- `Vector.h`
  - 템플릿 벡터 클래스 `Vector<N, T>`
  - 기본 연산 (`+`, `-`, 스칼라 `*`)
  - 내적 `Dot`, 3D 외적 `Cross`
  - 별칭 타입 (`Vec2`, `Vec3`, `Vec4`)
- `Matrix.h/.cpp`
  - 동적 크기 `Matrix` 클래스
  - `Identity`, 행렬 덧셈/뺄셈/곱셈
  - `Matrix(4x4) * Vec4` 연산

## 의도

이 구조를 기반으로 이후 다음 기능을 확장하기 쉽도록 설계했습니다.

- 고정 크기 행렬 (`Mat3`, `Mat4`)
- 변환 행렬(Translate/Rotate/Scale)
- 쿼터니언
- SIMD 최적화
- 단위 테스트 체계

