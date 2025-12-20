#pragma once
#include <3DEngine/Utility/SIMDVec3.hpp>

struct SIMDParticle {
  int id;
  double mass;
  double radius;
  SIMDVec3 pos;
  SIMDVec3 vel;

  SIMDParticle() : id(-1), mass(1.0), radius(1.0), pos(), vel() {}
  SIMDParticle(int i, double m, double r, SIMDVec3 p, SIMDVec3 v) : id(i), mass(m), radius(r), pos(p), vel(v) {}
};

namespace ParticleUtil {}
