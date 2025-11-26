#pragma once
#include <immintrin.h>
#include <cmath>

struct SIMDVec3 {
  union {
    struct { double x, y, z, _pad; };
    __m256d simd;
  };

  SIMDVec3() : simd(_mm256_setzero_pd()) {}

  SIMDVec3(double xx, double yy, double zz) : simd(_mm256_set_pd(0.0, zz, yy, xx)) {}

  SIMDVec3(__m256d v) : simd(v) {}

  SIMDVec3& operator+=(SIMDVec3 const& o) {
    simd = _mm256_add_pd(simd, o.simd);
    return *this;
  }

  SIMDVec3 operator+(SIMDVec3 const& o) const {
    return SIMDVec3(_mm256_add_pd(simd, o.simd));
  }

  SIMDVec3 operator-(SIMDVec3 const& o) const {
    return SIMDVec3(_mm256_sub_pd(simd, o.simd));
  }

  SIMDVec3 operator*(double s) const {
    __m256d scalar = _mm256_set1_pd(s);
    return SIMDVec3(_mm256_mul_pd(simd, scalar));
  }

  SIMDVec3 operator/(double s) const {
    __m256d scalar = _mm256_set1_pd(s);
    return SIMDVec3(_mm256_div_pd(simd, scalar));
  }
};

namespace SIMDVec3Util {
static constexpr double WORLD_SIZE = 1'000'000.0;

inline double dot(SIMDVec3 const& a, SIMDVec3 const& b) {
  __m256d mul = _mm256_mul_pd(a.simd, b.simd);

  __m128d low = _mm256_castpd256_pd128(mul);
  __m128d high = _mm256_extractf128_pd(mul, 1);

  __m128d sum = _mm_add_pd(low, high);
  __m128d shuf = _mm_shuffle_pd(sum, sum, 1);
  __m128d result = _mm_add_sd(sum, shuf);

  return _mm_cvtsd_f64(result);
}

inline double lengthSq(SIMDVec3 const& v) {
  return dot(v, v);
}

inline double length(SIMDVec3 const& v) {
  return std::sqrt(lengthSq(v));
}

inline SIMDVec3 normalize(SIMDVec3 const& v) {
  double len = length(v);
  const double eps = 1e-12;
  if (len <= eps)
    return SIMDVec3();
  return v / len;
}

inline SIMDVec3 minimum_image_delta(const SIMDVec3& p1, const SIMDVec3& p2) {
  // delta = p1 - p2
  __m256d delta = _mm256_sub_pd(p1.simd, p2.simd);

  __m256d half = _mm256_set1_pd(WORLD_SIZE * 0.5);
  __m256d world = _mm256_set1_pd(WORLD_SIZE);
  __m256d neg_half = _mm256_set1_pd(-WORLD_SIZE * 0.5);


  __m256d gt_half = _mm256_cmp_pd(delta, half, _CMP_GT_OQ);
  __m256d le_neg_half = _mm256_cmp_pd(delta, neg_half, _CMP_LE_OQ);


  __m256d correction1 = _mm256_and_pd(gt_half, world);
  delta = _mm256_sub_pd(delta, correction1);


  __m256d correction2 = _mm256_and_pd(le_neg_half, world);
  delta = _mm256_add_pd(delta, correction2);

  return SIMDVec3(delta);
}

inline double wrap_coord(double x) {
  double r = std::fmod(x, WORLD_SIZE);
  if (r < 0) r += WORLD_SIZE;
  return r;
}

inline SIMDVec3 wrap_pos(const SIMDVec3& p) {
  return SIMDVec3(wrap_coord(p.x), wrap_coord(p.y), wrap_coord(p.z));
}

}
