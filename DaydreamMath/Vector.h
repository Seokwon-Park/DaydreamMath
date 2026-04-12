#pragma once

namespace Daydream
{
	// 1. 기본 템플릿 선언 (사용되지 않고 특수화 구조를 강제함)
	template <size_t L, typename T>
	struct Vec;

	// 2. 3차원 벡터 특수화 (Vec<3, T>)
	template <typename T>
	struct alignas(16) Vec<3, T> {
		union {
			struct {
				T x, y, z;
				T pad; // SIMD 정렬을 위한 16바이트 맞춤용 패딩
			};
			T data[4];
		};

		// 생성자
		Vec() : x(0), y(0), z(0), pad(0) {}
		Vec(T _x, T _y, T _z) : x(_x), y(_y), z(_z), pad(0) {}
		explicit Vec(T _scalar) : x(_scalar), y(_scalar), z(_scalar), pad(0) {}

		// 기본 함수 (대문자 시작)
		static Vec Zero() { return Vec(0, 0, 0); }
		static Vec One() { return Vec(1, 1, 1); }

		// 데이터 접근
		T* Data() { return data; }
		const T* Data() const { return data; }

		T& operator[](size_t _index) { return data[_index]; }
		const T& operator[](size_t _index) const { return data[_index]; }

		// 연산자 오버로딩
		Vec& operator+=(const Vec& _other) {
			x += _other.x;
			y += _other.y;
			z += _other.z;
			return *this;
		}
	};

	// 비멤버 연산자
	template <typename T>
	inline Vec<3, T> operator+(const Vec<3, T>& _a, const Vec<3, T>& _b) {
		return Vec<3, T>(_a.x + _b.x, _a.y + _b.y, _a.z + _b.z);
	}

	// 3. 엔진 내에서 사용할 직관적인 타입 별칭 (Aliasing)
	using Vector3 = Vec<3, float>;
	using Vector3i = Vec<3, int>;
	using Vector3d = Vec<3, double>; // 필요하다면 쉽게 확장 가능
} // namespace Daydream
