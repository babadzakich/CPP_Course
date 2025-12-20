#pragma once
#include <cmath>

struct Vec3 {
  double x, y, z;
  Vec3() : x(0), y(0), z(0) {}
  Vec3(double xx, double yy, double zz) : x(xx), y(yy), z(zz) {}
  Vec3& operator+=(Vec3 const& o) {
    x += o.x;
    y += o.y;
    z += o.z;
    return *this;
  }
  Vec3 operator+(Vec3 const& o) const { return Vec3(x + o.x, y + o.y, z + o.z); }
  Vec3 operator-(Vec3 const& o) const { return Vec3(x - o.x, y - o.y, z - o.z); }
  Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }
  Vec3 operator/(double s) const { return Vec3(x / s, y / s, z / s); }
};

namespace Vec3Util {
static constexpr double WORLD_SIZE = 1'000'000.0;
inline double dot(Vec3 const& a, Vec3 const& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
inline double lengthSq(Vec3 const& v) {
  return dot(v, v);
}
inline double length(Vec3 const& v) {
  return std::sqrt(lengthSq(v));
}
inline Vec3 normalize(Vec3 const& v) {
  double len = length(v);
  const double eps = 1e-12;
  if (len <= eps)
    return Vec3(0, 0, 0);
  return v / len;
}
inline Vec3 minimum_image_delta(const Vec3& p1, const Vec3& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double dz = p1.z - p2.z;

    // если delta > L/2, корректируем на -L; если < -L/2, корректируем на +L
    double half = WORLD_SIZE * 0.5;
    if (dx > half) dx -= WORLD_SIZE;
    else if (dx <= -half) dx += WORLD_SIZE;

    if (dy > half) dy -= WORLD_SIZE;
    else if (dy <= -half) dy += WORLD_SIZE;

    if (dz > half) dz -= WORLD_SIZE;
    else if (dz <= -half) dz += WORLD_SIZE;

    return Vec3(dx, dy, dz);
}

inline double wrap_coord(double x) {
    // возвращаем в диапазон [0, WORLD_SIZE)
    double r = std::fmod(x, WORLD_SIZE);
    if (r < 0) r += WORLD_SIZE;
    return r;
}

inline Vec3 wrap_pos(const Vec3& p) {
    return Vec3(wrap_coord(p.x), wrap_coord(p.y), wrap_coord(p.z));
}
}  // namespace Vec3Util
