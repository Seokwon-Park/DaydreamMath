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

		inline Bool Compare(SIMDRegister _a, SIMDRegister _b) 
		{
			__m128 cmp = _mm_cmpeq_ps(_a, _b);

			// 2. _mm_movemask_ps: 128비트 결과를 4비트 정수(Int)로 압축해 줍니다!
			// 4개가 모두 같았다면 0b1111 (십진수로 15, 16진수로 0xF)가 나옵니다.
			return _mm_movemask_ps(cmp) == 0xF;
		}

		inline Bool IsNearlyEqual(__m128 _a, __m128 _b, Float32 _epsilon)
		{
			// Step 1: 뺄셈 (A - B)
			__m128 diff = _mm_sub_ps(_a, _b);

			// Step 2: abs
			// 0x7FFFFFFF (이진수로 01111111...) 마스크 생성
			// 이것과 AND 연산을 하면 맨 앞의 부호 비트가 무조건 0(양수)
			// _mm_castsi128_ps - 다시 float4 형태로 캐스팅
			__m128 absMask = _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF));
			__m128 absDiff = _mm_and_ps(diff, absMask);

			// Step 3: 엡실론 레지스터 준비 (4칸 모두 엡실론 값으로 꽉 채움)
			__m128 epsReg = _mm_set1_ps(_epsilon);

			// Step 4: 작거나 같은가? (Compare Less-than or Equal)
			// absDiff <= epsReg 를 검사하여 맞으면 0xFFFFFFFF, 틀리면 0x0 을 반환
			__m128 cmp = _mm_cmple_ps(absDiff, epsReg);

			// Step 5: 4개의 결과가 모두 0xFFFFFFFF(True) 인지 압축해서 확인
			return _mm_movemask_ps(cmp) == 0xF;
		}

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

		inline Bool Compare(SIMDRegister _a, SIMDRegister _b)
		{
			// 1. 비교 연산: SSE와 마찬가지로 같으면 0xFFFFFFFF, 다르면 0x00000000 이 들어갑니다.
			uint32x4_t cmp = vceqq_f32(_a, _b);

			// 2. Movemask의 대체제: "모든 것이 0xFFFFFFFF인가?" 를 검사하는 기적의 꼼수
			return vminvq_u32(cmp) == 0xFFFFFFFF;


			// =======================================================
			// (vminvq_u32는 64비트 전용 명령어라 구형 폰에선 에러가 납니다)
			// =======================================================
			/*
			// 128비트(4개)를 64비트(2개)씩 쪼개서 비트 AND 연산을 때립니다.
			uint32x2_t and_half = vand_u32(vget_low_u32(cmp), vget_high_u32(cmp));

			// 남은 2개를 각각 뽑아서 한번 더 AND 연산!
			uint32_t final_mask = vget_lane_u32(and_half, 0) & vget_lane_u32(and_half, 1);

			return final_mask == 0xFFFFFFFF;
			*/
		}

		inline Bool IsNearlyEqual(float32x4_t _a, float32x4_t _b, Float32 _epsilon)
		{
			// Step 1: 뺄셈 (A - B)
			float32x4_t diff = vsubq_f32(_a, _b);

			// Step 2: 절대값 구하기
			// NEON은 전용 명령어(vabsq)가 있어서 흑마법을 쓸 필요가 없습니다!
			float32x4_t absDiff = vabsq_f32(diff);

			// Step 3: 엡실론 레지스터 준비 (4칸 복사)
			float32x4_t epsReg = vdupq_n_f32(_epsilon);

			// Step 4: 작거나 같은가? (Compare Less-than or Equal)
			uint32x4_t cmp = vcleq_f32(absDiff, epsReg);

			// Step 5: 이전 답변에서 배웠던 기적의 꼼수 (가장 작은 값이 0xFFFFFFFF인가?)
			return vminvq_u32(cmp) == 0xFFFFFFFF;
		}

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