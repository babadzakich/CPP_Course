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

  spheres.resize(spheresAmount);
  file.read(reinterpret_cast<char*>(spheres.data()), spheresAmount * sizeof(Particle));
}

void MultithreadEngine::save_to_text(const std::string& filename) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + filename);
  }
  file.write(reinterpret_cast<const char*>(spheres.data()), spheresAmount * sizeof(Particle));
}

void MultithreadEngine::step(double dt) {
  std::vector<std::thread> threads;
  // Update positions in parallel
  {
    size_t chunk_size = (spheresAmount + num_threads - 1) / num_threads;

    // Position update
    for (size_t t = 0; t < num_threads; ++t) {
      threads.emplace_back([this, t, chunk_size, dt]() {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, spheresAmount);
        for (size_t i = start; i < end; ++i) {
          spheres[i].pos += spheres[i].vel * dt;
          spheres[i].pos = Vec3Util::wrap_pos(spheres[i].pos);
        }
      });
    }

    for (auto& th : threads) {
      th.join();
    }
    threads.clear();
  }

  const size_t batch_size = 50;  // Обрабатываем по 50 сфер за раз

  for (size_t batch_start = 0; batch_start < spheresAmount; batch_start += batch_size) {
    size_t batch_end = std::min(batch_start + batch_size, spheresAmount);

    // Создаём список всех пар в этом батче
    struct PairIndex {
      size_t i, j;
    };
    std::vector<PairIndex> pairs;

    for (size_t i = batch_start; i < batch_end; ++i) {
      for (size_t j = i + 1; j < spheresAmount; ++j) {
        pairs.push_back({i, j});
      }
    }

    // Если пар мало, обрабатываем последовательно
    if (pairs.size() < num_threads * 2) {
      for (const auto& p : pairs) {
        if (checkCollision(spheres[p.i], spheres[p.j])) {
          resolveCollisions(spheres[p.i], spheres[p.j]);
        }
      }
      continue;
    }

    std::atomic<size_t> current_pair{0};

    for (size_t t = 0; t < num_threads; ++t) {
      threads.emplace_back([this, &current_pair, &pairs]() {
        while (true) {
          size_t idx = current_pair.fetch_add(1, std::memory_order_relaxed);
          if (idx >= pairs.size()) break;

          size_t i = pairs[idx].i;
          size_t j = pairs[idx].j;

          if (checkCollision(spheres[i], spheres[j])) {
            resolveCollisions(spheres[i], spheres[j]);
          }
        }
      });
    }

    for (auto& th : threads) {
      th.join();
    }
    threads.clear();
  }
}

bool MultithreadEngine::checkCollision(const Particle& p1, const Particle& p2) {
  Vec3 delta = Vec3Util::minimum_image_delta(p1.pos, p2.pos);
  double distSquared = Vec3Util::lengthSq(delta);
  double radiusSum = p1.radius + p2.radius;
  return distSquared <= radiusSum * radiusSum;
}

void MultithreadEngine::resolveCollisions(Particle& p1, Particle& p2) {
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
