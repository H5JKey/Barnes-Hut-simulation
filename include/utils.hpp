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

    inline float computeSquaredLength(const sf::Vector2f& vec) noexcept{
    #if USE_SSE_INTRINSICS
        __m128 vec_ = _mm_setr_ps(vec.x, vec.y, 0.0f, 0.0f);
        __m128 sq = _mm_mul_ps(vec_, vec_);
        __m128 sum = _mm_hadd_ps(sq, sq);
        return _mm_cvtss_f32(sum);
    #else 
        float len = vec.x * vec.x + vec.y * vec.y;
        return len;
    #endif
    }

    inline float computeLength(const sf::Vector2f& vec) noexcept{
    #if USE_SSE_INTRINSICS
        __m128 vec_ = _mm_setr_ps(vec.x, vec.y, 0.0f, 0.0f);
        __m128 sq = _mm_mul_ps(vec_, vec_);
        __m128 sum = _mm_hadd_ps(sq, sq);
        __m128 invSqrt = _mm_rsqrt_ss(sum);
        __m128 result = _mm_mul_ss(sum, invSqrt);
        float final_result;
        _mm_store_ss(&final_result, result);
        return final_result;
    #else 
        float len = std::sqrt(vec.x * vec.x + vec.y * vec.y);
        return len;
    #endif
    }
}