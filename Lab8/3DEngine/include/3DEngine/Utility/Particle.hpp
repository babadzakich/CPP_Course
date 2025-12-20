#pragma once
#include <3DEngine/Utility/Vec3.hpp>

struct Particle {
  int id;
  double mass;
  double radius;
  Vec3 pos;
  Vec3 vel;

  Particle() : id(-1), mass(1.0), radius(1.0), pos(), vel() {}
  Particle(int i, double m, double r, Vec3 p, Vec3 v) : id(i), mass(m), radius(r), pos(p), vel(v) {}
};

namespace ParticleUtil {}
