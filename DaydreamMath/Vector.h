#pragma once

#include "Fwd.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "SIMD.h"

namespace Daydream
{
    template<UInt64 N, typename T>
    inline T Dot(const Vector<N, T>& _a, const Vector<N, T>& _b)
    {
        T result = 0;
        for (int i = 0; i < N; ++i) result += _a[i] * _b[i];
        return result;
    }

    template<>
    inline Float32 Dot(const Vector<4, Float32>& _a, const Vector<4, Float32>& _b)
    {
        //return SIMD::Dot(_a.reg, _b.reg);
        return 0;
    }
}
