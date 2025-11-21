#include <3DEngine/MultithreadEngine/MultithreadEngine.hpp>
#include <fstream>
#include <thread>
#include <vector>

void MultithreadEngine::init_from_text(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for reading: " + filename);
  }
  spheres.clear();

  file.read(reinterpret_cast<char*>(&spheresAmount), sizeof(spheresAmount));
  spheres.resize(spheresAmount);
  file.read(reinterpret_cast<char*>(spheres.data()), spheresAmount * sizeof(Particle));
}

void MultithreadEngine::save_to_text(const std::string& filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filename);
  }
  file.write(reinterpret_cast<const char*>(&spheresAmount), sizeof(spheresAmount));
  file.write(reinterpret_cast<const char*>(spheres.data()), spheresAmount * sizeof(Particle));
}

void MultithreadEngine::step(double dt) {
  // Update positions in parallel
  size_t chunk_size = (spheresAmount + num_threads - 1) / num_threads;
  std::vector<std::thread> threads;

  // Position update
  for (size_t t = 0; t < num_threads; ++t) {
    threads.emplace_back([this, t, chunk_size, dt]() {
      size_t start = t * chunk_size;
      size_t end = std::min(start + chunk_size, spheresAmount);
      for (size_t i = start; i < end; ++i) {
        spheres[i].pos += spheres[i].vel * dt;
      }
    });
  }

  for (auto& th : threads) {
    th.join();
  }
  threads.clear();

  // Collision detection and resolution (need synchronization)
  for (size_t i = 0; i < spheresAmount; ++i) {
    for (size_t j = i + 1; j < spheresAmount; ++j) {
      if (checkCollision(spheres[i], spheres[j])) {
        resolveCollisions(spheres[i], spheres[j]);
      }
    }
  }
}

bool MultithreadEngine::checkCollision(const Particle& p1, const Particle& p2) {
  Vec3 delta = p1.pos - p2.pos;
  double distSquared = Vec3Util::lengthSq(delta);
  double radiusSum = p1.radius + p2.radius;
  return distSquared <= radiusSum * radiusSum;
}

void MultithreadEngine::resolveCollisions(Particle& p1, Particle& p2) {
  Vec3 normal = Vec3Util::normalize(p1.pos - p2.pos);
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

  Vec3 diff = p1.pos - p2.pos;
  double distance = Vec3Util::length(diff);
  double overlap = (p1.radius + p2.radius) - distance;

  if (overlap > 0) {
    Vec3 separation = normal * (overlap / 2);
    p1.pos = p1.pos + separation;
    p2.pos = p2.pos - separation;
  }
}

std::vector<Particle> MultithreadEngine::snapshot() const {
  return spheres;
}

ConservationValues MultithreadEngine::compute_conserv() const {
  ConservationValues cv;
  Vec3 total_momentum(0, 0, 0);

  for (const auto& p : spheres) {
    total_momentum += p.vel * p.mass;
    cv.total_kinetic_energy += 0.5 * p.mass * Vec3Util::lengthSq(p.vel);
  }

  cv.total_momentum_magnitude = Vec3Util::length(total_momentum);
  return cv;
}
