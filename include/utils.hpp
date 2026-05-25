#pragma once

#if defined(__SSE__) || defined(_M_IX86) || defined(_M_X64)
#define USE_SSE_INTRINSICS 1
#else
#define USE_SSE_INTRINSICS 0
#endif

#include <SFML/Graphics.hpp>
#include <cmath>
#if USE_SSE_INTRINSICS
#include <immintrin.h>
#endif


namespace Utils {
    inline float fastSqrt(float x) noexcept {
    #if USE_SSE_INTRINSICS
        __m128 in = _mm_set_ss(x);
        __m128 invSqrt = _mm_rsqrt_ss(in);
        __m128 result = _mm_mul_ss(in, invSqrt);
        float final_result;
        _mm_store_ss(&final_result, result);
        return final_result;
    #else
        return sqrt(x);
    #endif
    }

    inline float fastInvSqrt(float x) noexcept {
    #if USE_SSE_INTRINSICS
        __m128 in = _mm_set_ss(x);
        __m128 result = _mm_rsqrt_ss(in);
        float final_result;
        _mm_store_ss(&final_result, result);
        return final_result;
    #else
        return rsqrtf32(x);
    #endif
    }

    inline float computeSquaredLength(float vec_x, float vec_y) noexcept{
        float len = vec_x * vec_x + vec_y * vec_y;
        return len;
    }

    inline float computeLength(float vec_x, float vec_y) noexcept{
        float len = fastSqrt(vec_x * vec_x + vec_y * vec_y);
        return len;
    }
}