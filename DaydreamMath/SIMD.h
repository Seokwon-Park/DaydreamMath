#pragma once

#include "BaseTypes.h"

#if defined(DAYDREAM_FORCE_PURE)
// 사용자가 강제로 SIMD를 껐을 때
#define DAYDREAM_SIMD_NONE
#elif defined(__x86_64__) || defined(_M_X64)
// 인텔/AMD 환경
#define DAYDREAM_SIMD_SSE
#include <immintrin.h>
#elif defined(__aarch64__) || defined(_M_ARM64)
// 애플 실리콘/모바일 환경
#define DAYDREAM_SIMD_NEON
#include <arm_neon.h>
#else
#define DAYDREAM_SIMD_NONE
#endif

namespace Daydream
{
	// 2. SIMD 타입 정의
#if defined(__x86_64__) || defined(_M_X64)
	using SIMDRegister = __m128;
#elif defined(__aarch64__) || defined(_M_ARM64)
	using SIMDRegister = float32x4_t;
#else
	struct alignas(16) SIMDRegister { float v[4]; };
#endif

	namespace SIMD
	{
#if defined(DAYDREAM_SIMD_SSE)
		// SSE 구현
		inline SIMDRegister SetZero() { return _mm_setzero_ps(); }

		inline SIMDRegister Add(SIMDRegister _a, SIMDRegister _b) { return _mm_add_ps(_a, _b); }
		inline SIMDRegister Sub(SIMDRegister _a, SIMDRegister _b) { return _mm_sub_ps(_a, _b); }
		inline SIMDRegister Mul(SIMDRegister _a, SIMDRegister _b) { return _mm_mul_ps(_a, _b); }
		inline SIMDRegister SetVal(Float32 _val) { return _mm_set1_ps(_val); }
		inline SIMDRegister SetVal(Float32 _x, Float32 _y, Float32 _z, Float32 _w) { return _mm_setr_ps(_x, _y, _z, _w); }

		// 나눗셈 및 근사
		inline SIMDRegister Div(SIMDRegister _a, SIMDRegister _b) { return _mm_div_ps(_a, _b); }
		inline SIMDRegister Rsqrt(SIMDRegister _val) { return _mm_rsqrt_ps(_val); }

		// 비교 및 선택 (Clamp 구현 시 필수)
		inline SIMDRegister Min(SIMDRegister _a, SIMDRegister _b) { return _mm_min_ps(_a, _b); }
		inline SIMDRegister Max(SIMDRegister _a, SIMDRegister _b) { return _mm_max_ps(_a, _b); }

		// 내적 (SSE 4.1 이상)
		inline Float32 Dot(SIMDRegister _a, SIMDRegister _b)
		{
			SIMDRegister res = _mm_dp_ps(_a, _b, 0xFF);
			return _mm_cvtss_f32(res);
		}

		// 데이터 재배치 (외적 구현 시 필수)
		// 셔플은 매크로를 사용해야 하므로 템플릿으로 감싸는 것이 일반적
		template<int X, int Y, int Z, int W>
		inline SIMDRegister Shuffle(SIMDRegister _a, SIMDRegister _b)
		{
			return _mm_shuffle_ps(_a, _b, _MM_SHUFFLE(W, Z, Y, X));
		}

		template<int X, int Y, int Z, int W>
		inline SIMDRegister Swizzle(SIMDRegister _a)
		{
			return _mm_shuffle_ps(_a, _a, _MM_SHUFFLE(W, Z, Y, X));
		}

		// X, Y, Z, W 중 하나를 4칸에 모두 복사 (행렬 곱셈 필수품)
		inline SIMDRegister SplatX(SIMDRegister _a) { return Swizzle<0, 0, 0, 0>(_a); }
		inline SIMDRegister SplatY(SIMDRegister _a) { return Swizzle<1, 1, 1, 1>(_a); }
		inline SIMDRegister SplatZ(SIMDRegister _a) { return Swizzle<2, 2, 2, 2>(_a); }
		inline SIMDRegister SplatW(SIMDRegister _a) { return Swizzle<3, 3, 3, 3>(_a); }

		inline SIMDRegister And(SIMDRegister _a, SIMDRegister _b) { return _mm_and_ps(_a, _b); }
		inline SIMDRegister Or(SIMDRegister _a, SIMDRegister _b) { return _mm_or_ps(_a, _b); }
		inline SIMDRegister Xor(SIMDRegister _a, SIMDRegister _b) { return _mm_xor_ps(_a, _b); }
		inline SIMDRegister AndNot(SIMDRegister _a, SIMDRegister _b) { return _mm_andnot_ps(_a, _b); } // ~a & b

		inline SIMDRegister LoadAligned(const Float32* _ptr) { return _mm_load_ps(_ptr); }
		inline void StoreAligned(Float32* _ptr, SIMDRegister _val) { _mm_store_ps(_ptr, _val); }

		// 정렬되지 않은 메모리에서 읽기/쓰기 (조금 느리지만 안전)
		inline SIMDRegister LoadUnaligned(const Float32* _ptr) { return _mm_loadu_ps(_ptr); }
		inline void StoreUnaligned(Float32* _ptr, SIMDRegister _val) { _mm_storeu_ps(_ptr, _val); }

		// result = (_a * _b) + _c
		inline SIMDRegister MultiplyAdd(SIMDRegister _a, SIMDRegister _b, SIMDRegister _c)
		{
#ifdef DAYDREAM_SIMD_FMA // (나중에 AVX2/FMA 지원을 켤 경우)
			return _mm_fmadd_ps(_a, _b, _c);
#else
			return Add(Mul(_a, _b), _c);
#endif
		}

#elif defined(DAYDREAM_SIMD_NEON)
		inline SIMDRegister SetZero() { return vdupq_n_f32(0.0f); }

		inline SIMDRegister Add(SIMDRegister _a, SIMDRegister _b) { return vaddq_f32(_a, _b); }
		inline SIMDRegister Sub(SIMDRegister _a, SIMDRegister _b) { return vsubq_f32(_a, _b); }
		inline SIMDRegister Mul(SIMDRegister _a, SIMDRegister _b) { return vmulq_f32(_a, _b); }
		inline SIMDRegister SetVal(Float32 _val) { return vdupq_n_f32(_val); }

		// SSE의 _mm_setr_ps와 완벽히 동일한 메모리 순서를 보장하려면 배열 로드가 가장 안전합니다.
		inline SIMDRegister SetVal(Float32 _x, Float32 _y, Float32 _z, Float32 _w)
		{
			alignas(16) Float32 data[4] = { _x, _y, _z, _w };
			return vld1q_f32(data);
		}

		// 2. 나눗셈 및 근사
		inline SIMDRegister Div(SIMDRegister _a, SIMDRegister _b) { return vdivq_f32(_a, _b); }

		// NEON은 역제곱근 계산 시, 추정치(Estimate)를 구하고 뉴턴-랩슨(Newton-Raphson) 
		// 1단계를 거쳐야 SSE와 비슷한 수준의 정밀도
		inline SIMDRegister Rsqrt(SIMDRegister _val)
		{
			SIMDRegister estimate = vrsqrteq_f32(_val);
			estimate = vmulq_f32(vrsqrtsq_f32(vmulq_f32(_val, estimate), estimate), estimate);
			return estimate;
		}

		// 3. 비교 및 선택
		inline SIMDRegister Min(SIMDRegister _a, SIMDRegister _b) { return vminq_f32(_a, _b); }
		inline SIMDRegister Max(SIMDRegister _a, SIMDRegister _b) { return vmaxq_f32(_a, _b); }

		// 4. 내적 (Dot Product)
		inline Float32 Dot(SIMDRegister _a, SIMDRegister _b)
		{
			SIMDRegister mul = vmulq_f32(_a, _b);
			// ARMv8(64비트) 최강의 명령어: 레지스터 안의 4개 값을 한방에 더해줍니다.
			return vaddvq_f32(mul);
		}

		// NEON은 SSE의 _mm_shuffle_ps 같은 깔끔한 매크로가 없습니다. 
		// 상용 엔진(언리얼 등)은 Clang/GCC의 내장 함수를 사용하는 것을 표준으로 삼습니다.
		template<int X, int Y, int Z, int W>
		inline SIMDRegister Shuffle(SIMDRegister _a, SIMDRegister _b)
		{
			return __builtin_shufflevector(_a, _b, X, Y, Z, W + 4);
		}

		template<int X, int Y, int Z, int W>
		inline SIMDRegister Swizzle(SIMDRegister _a)
		{
			return __builtin_shufflevector(_a, _a, X, Y, Z, W);
		}

		// 특정 위치의 값을 4칸에 쫙 뿌리는(Splat) NEON 전용 명령어
		inline SIMDRegister SplatX(SIMDRegister _a) { return vdupq_laneq_f32(_a, 0); }
		inline SIMDRegister SplatY(SIMDRegister _a) { return vdupq_laneq_f32(_a, 1); }
		inline SIMDRegister SplatZ(SIMDRegister _a) { return vdupq_laneq_f32(_a, 2); }
		inline SIMDRegister SplatW(SIMDRegister _a) { return vdupq_laneq_f32(_a, 3); }

		// NEON은 Float 타입에 대한 비트 연산 명령어가 없어서, int형으로 캐스팅 후 계산하고 롤백
		inline SIMDRegister And(SIMDRegister _a, SIMDRegister _b) {
			return vreinterpretq_f32_u32(vandq_u32(vreinterpretq_u32_f32(_a), vreinterpretq_u32_f32(_b)));
		}
		inline SIMDRegister Or(SIMDRegister _a, SIMDRegister _b) {
			return vreinterpretq_f32_u32(vorrq_u32(vreinterpretq_u32_f32(_a), vreinterpretq_u32_f32(_b)));
		}
		inline SIMDRegister Xor(SIMDRegister _a, SIMDRegister _b) {
			return vreinterpretq_f32_u32(veorq_u32(vreinterpretq_u32_f32(_a), vreinterpretq_u32_f32(_b)));
		}
		inline SIMDRegister AndNot(SIMDRegister _a, SIMDRegister _b) {
			// SSE의 _mm_andnot_ps(a, b) 는 (~a) & b
			// 하지만 NEON의 vbicq_u32(x, y) 는 x & (~y)
			// 따라서 파라미터 순서를 b, a로 뒤집어줘야 완벽하게 동일한 결과
			return vreinterpretq_f32_u32(vbicq_u32(vreinterpretq_u32_f32(_b), vreinterpretq_u32_f32(_a)));
		}

		// 모던 ARM은 vld1q_f32 자체가 정렬 여부를 알아서 처리하므로 동일하게 사용
		inline SIMDRegister LoadAligned(const Float32* _ptr) { return vld1q_f32(_ptr); }
		inline void StoreAligned(Float32* _ptr, SIMDRegister _val) { vst1q_f32(_ptr, _val); }
		inline SIMDRegister LoadUnaligned(const Float32* _ptr) { return vld1q_f32(_ptr); }
		inline void StoreUnaligned(Float32* _ptr, SIMDRegister _val) { vst1q_f32(_ptr, _val); }

		// SSE는 _mm_fmadd_ps(_a, _b, _c) = (a * b) + c 순서지만
		// NEON의 vfmaq_f32(_c, _a, _b) = c + (a * b) 순서로 인자를 받음
		inline SIMDRegister MultiplyAdd(SIMDRegister _a, SIMDRegister _b, SIMDRegister _c)
		{
			return vfmaq_f32(_c, _a, _b);
		}

#elif defined(DAYDREAM_SIMD_NONE)

#endif
	}
}