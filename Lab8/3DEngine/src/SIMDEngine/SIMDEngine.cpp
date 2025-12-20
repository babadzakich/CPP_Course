#include <3DEngine/SIMDEngine/SIMDEngine.hpp>
#include <fstream>

void SIMDEngine::init_from_text(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for reading: " + filename);
  }
  SIMDspheres.clear();
  int id;
  double x, y, z, vx, vy, vz, mass, radius;
  for (int i = 0; i < spheresAmount; i++) {
    if (!(file >> id >> x >> y >> z >> vx >> vy >> vz >> mass >> radius))
      throw std::runtime_error("Not enough spheres");
    SIMDParticle p(id, mass, radius, SIMDVec3{x, y, z}, SIMDVec3{vx, vy, vz});
    SIMDspheres.push_back(p);
  }
  calculate_conservation();
}

void SIMDEngine::save_to_text(const std::string& filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filename);
  }
  for (const auto& p : SIMDspheres) {
    file << p.id << " " << p.pos.x << " " << p.pos.y << " " << p.pos.z << " " << p.vel.x << " " << p.vel.y << " " << p.vel.z << " "
         << p.mass << " " << p.radius << "\n";
  }
}

std::vector<Particle> SIMDEngine::snapshot() const {
  std::vector<Particle> snapshot(spheresAmount);
    for (auto& s : SIMDspheres) {
      snapshot[s.id].pos.x = s.pos.x;
      snapshot[s.id].pos.y = s.pos.y;
      snapshot[s.id].pos.z = s.pos.z;
      snapshot[s.id].vel.x = s.vel.x;
      snapshot[s.id].vel.y = s.vel.y;
      snapshot[s.id].vel.z = s.vel.z;
      snapshot[s.id].mass = s.mass;
      snapshot[s.id].radius = s.radius;
      snapshot[s.id].id = s.id;
    }
  return snapshot;
}

ConservationValues SIMDEngine::compute_conserv() const {
  ConservationValues cv;
  SIMDVec3 total_momentum(0, 0, 0);

  for (const auto& p : SIMDspheres) {
    total_momentum += p.vel * p.mass;
    cv.total_kinetic_energy += 0.5 * p.mass * SIMDVec3Util::lengthSq(p.vel);
  }

  cv.total_momentum_magnitude = SIMDVec3Util::length(total_momentum);
  return cv;
}

void SIMDEngine::step(double dt) {
  for (auto& p : SIMDspheres) {
    p.pos = SIMDVec3Util::wrap_pos(p.pos + (p.vel * dt));
    // p.pos += p.vel * dt;
  }

  for (size_t i = 0; i < SIMDspheres.size(); ++i) {
    for (size_t j = i + 1; j < SIMDspheres.size(); ++j) {
      if (checkCollision(SIMDspheres[i], SIMDspheres[j])) {
        resolveCollisions(SIMDspheres[i], SIMDspheres[j]);
      }
    }
  }
}

bool SIMDEngine::checkCollision(const SIMDParticle& p1, const SIMDParticle& p2) {
  SIMDVec3 delta = SIMDVec3Util::minimum_image_delta(p1.pos,p2.pos);
  double distSquared = SIMDVec3Util::lengthSq(delta);
  double radiusSum = p1.radius + p2.radius;
  return distSquared <= radiusSum * radiusSum;
}

void SIMDEngine::resolveCollisions(SIMDParticle& p1, SIMDParticle& p2) {
  SIMDVec3 delta = SIMDVec3Util::minimum_image_delta(p1.pos, p2.pos);
    double dist = SIMDVec3Util::length(delta);

  SIMDVec3 normal;
  if (dist > 1e-12) {
      normal = delta / dist;
  } else {
      normal = SIMDVec3(1.0, 0.0, 0.0);
      dist = 0;
  }


  SIMDVec3 relativevel = p1.vel - p2.vel;

  double velocityAlongNormal = SIMDVec3Util::dot(relativevel, normal);

  if (velocityAlongNormal > 0) {
    return;
  }

  double restitution = 1.0;

  double j = -(1 + restitution) * velocityAlongNormal;
  j /= (1 / p1.mass + 1 / p2.mass);

  SIMDVec3 impulse = normal * j;
  p1.vel = p1.vel + impulse * (1 / p1.mass);
  p2.vel = p2.vel - impulse * (1 / p2.mass);

  double overlap = (p1.radius + p2.radius) - dist;

  if (overlap > 0.0) {
    SIMDVec3 separation = normal * (overlap / 2);
    p1.pos = p1.pos + separation;
    p2.pos = p2.pos - separation;

    p1.pos = SIMDVec3Util::wrap_pos(p1.pos);
    p2.pos = SIMDVec3Util::wrap_pos(p2.pos);
  }
}
