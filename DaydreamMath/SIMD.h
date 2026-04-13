#pragma once

#include "Types.h"

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
		inline SIMDRegister Add(SIMDRegister _rhs, SIMDRegister _lhs) { return _mm_add_ps(_rhs, _lhs); }
		inline SIMDRegister Sub(SIMDRegister _rhs, SIMDRegister _lhs) { return _mm_sub_ps(_rhs, _lhs); }
		inline SIMDRegister Mul(SIMDRegister _rhs, SIMDRegister _lhs) { return _mm_mul_ps(_rhs, _lhs); }
		inline SIMDRegister Set1(Float32 _val) { return _mm_set1_ps(_val); }

		// 나눗셈 및 근사
		inline SIMDRegister Div(SIMDRegister _rhs, SIMDRegister _lhs) { return _mm_div_ps(_rhs, _lhs); }
		inline SIMDRegister Rsqrt(SIMDRegister _val) { return _mm_rsqrt_ps(_val); }

		// 비교 및 선택 (Clamp 구현 시 필수)
		inline SIMDRegister Min(SIMDRegister _rhs, SIMDRegister _lhs) { return _mm_min_ps(_rhs, _lhs); }
		inline SIMDRegister Max(SIMDRegister _rhs, SIMDRegister _lhs) { return _mm_max_ps(_rhs, _lhs); }

		// 내적 (SSE 4.1 이상)
		inline Float32 Dot(SIMDRegister _rhs, SIMDRegister _lhs)
		{
			SIMDRegister res = _mm_dp_ps(_rhs, _lhs, 0xFF);
			return _mm_cvtss_f32(res);
		}

		// 데이터 재배치 (외적 구현 시 필수)
		// 셔플은 매크로를 사용해야 하므로 템플릿으로 감싸는 것이 일반적입니다.
		template<int _x, int _y, int _z, int _w>
		inline SIMDRegister Shuffle(SIMDRegister _rhs, SIMDRegister _lhs)
		{
			return _mm_shuffle_ps(_rhs, _lhs, _MM_SHUFFLE(_w, _z, _y, _x));
		}

#elif defined(DAYDREAM_SIMD_NEON)
		// NEON 구현
		inline SIMDRegister Add(SIMDRegister _a, SIMDRegister _b) { return vaddq_f32(_a, _b); }
		inline SIMDRegister Sub(SIMDRegister _a, SIMDRegister _b) { return vsubq_f32(_a, _b); }
		inline SIMDRegister Mul(SIMDRegister _a, SIMDRegister _b) { return vmulq_f32(_a, _b); }
		inline SIMDRegister Set1(Float32 _val) { return vdupq_n_f32(_val); }

#elif defined(DAYDREAM_SIMD_NONE)

#endif
	}
}