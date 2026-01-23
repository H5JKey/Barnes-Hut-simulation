#if defined(__SSE__) || defined(_M_IX86) || defined(_M_X64)
#define USE_SSE_INTRINSICS 1
#else
#define USE_SSE_INTRINSICS 0
#endif

#if USE_SSE_INTRINSICS
#include <immintrin.h>
#endif

inline float fastSqrt(float x) noexcept {
#if USE_SSE_INTRINSICS
    __m128 in = _mm_set_ss(x);
    __m128 y_approx = _mm_rsqrt_ss(in);
    __m128 result = _mm_mul_ss(in, y_approx);
    float final_result;
    _mm_store_ss(&final_result, result);
    return final_result;
#else
    float xhalf = 0.5f * x;
    int i = *(int*)&x;
    i = 0x5f375a86 - (i >> 1);
    x = *(float*)&i;
    x = x * (1.5f - xhalf * x * x);
    return x;
#endif
}


