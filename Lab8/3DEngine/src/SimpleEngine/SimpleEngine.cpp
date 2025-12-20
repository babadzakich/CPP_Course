#include <3DEngine/SimpleEngine/SimpleEngine.hpp>
#include <fstream>

void SimpleEngine::init_from_text(const std::string& filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for reading: " + filename);
  }
  spheres.clear();
  int id;
  double x, y, z, vx, vy, vz, mass, radius;
  for (int i = 0; i < spheresAmount; i++) {
    if (!(file >> id >> x >> y >> z >> vx >> vy >> vz >> mass >> radius))
      throw std::runtime_error("Not enough spheres");
    Particle p(id, mass, radius, Vec3{x, y, z}, Vec3{vx, vy, vz});
    spheres.push_back(p);
  }
  calculate_conservation();
}

void SimpleEngine::save_to_text(const std::string& filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filename);
  }
  for (const auto& p : spheres) {
    file << p.id << " " << p.pos.x << " " << p.pos.y << " " << p.pos.z << " " << p.vel.x << " " << p.vel.y << " " << p.vel.z << " "
         << p.mass << " " << p.radius << "\n";
  }
}

std::vector<Particle> SimpleEngine::snapshot() const {
  return spheres;
}

ConservationValues SimpleEngine::compute_conserv() const {
  ConservationValues cv;
  Vec3 total_momentum(0, 0, 0);

  for (const auto& p : spheres) {
    total_momentum += p.vel * p.mass;
    cv.total_kinetic_energy += 0.5 * p.mass * Vec3Util::lengthSq(p.vel);
  }

  cv.total_momentum_magnitude = Vec3Util::length(total_momentum);
  return cv;
}

void SimpleEngine::step(double dt) {
  for (auto& p : spheres) {
    p.pos = Vec3Util::wrap_pos(p.pos + (p.vel * dt));
    // p.pos += p.vel * dt;
  }

  for (size_t i = 0; i < spheres.size(); ++i) {
    for (size_t j = i + 1; j < spheres.size(); ++j) {
      if (checkCollision(spheres[i], spheres[j])) {
        resolveCollisions(spheres[i], spheres[j]);
      }
    }
  }
}

bool SimpleEngine::checkCollision(const Particle& p1, const Particle& p2) {
  Vec3 delta = Vec3Util::minimum_image_delta(p1.pos,p2.pos);
  double distSquared = Vec3Util::lengthSq(delta);
  double radiusSum = p1.radius + p2.radius;
  return distSquared <= radiusSum * radiusSum;
}

void SimpleEngine::resolveCollisions(Particle& p1, Particle& p2) {
  Vec3 delta = Vec3Util::minimum_image_delta(p1.pos, p2.pos);
    double dist = Vec3Util::length(delta);

  Vec3 normal;
  if (dist > 1e-12) {
      normal = delta / dist;
  } else {
      normal = Vec3(1.0, 0.0, 0.0);
      dist = 0;
  }


  Vec3 relativevel = p1.vel - p2.vel;

  double velocityAlongNormal = Vec3Util::dot(relativevel, normal);

  if (velocityAlongNormal > 0) {
    return;
  }

  double restitution = 1.0;

  double j = -(1 + restitution) * velocityAlongNormal;
  j /= (1 / p1.mass + 1 / p2.mass);

  Vec3 impulse = normal * j;
  p1.vel = p1.vel + impulse * (1 / p1.mass);
  p2.vel = p2.vel - impulse * (1 / p2.mass);

  double overlap = (p1.radius + p2.radius) - dist;

  if (overlap > 0.0) {
    Vec3 separation = normal * (overlap / 2);
    p1.pos = p1.pos + separation;
    p2.pos = p2.pos - separation;

    p1.pos = Vec3Util::wrap_pos(p1.pos);
    p2.pos = Vec3Util::wrap_pos(p2.pos);
  }
}
