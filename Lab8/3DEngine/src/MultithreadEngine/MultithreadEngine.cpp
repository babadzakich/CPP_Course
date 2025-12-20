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

MultithreadEngine::CollisionPair MultithreadEngine::computeCollisionResponse(
    const Particle& p1, const Particle& p2, size_t idx1, size_t idx2) {

  CollisionPair cp;
  cp.i = idx1;
  cp.j = idx2;

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
    cp.impulse_i = Vec3(0, 0, 0);
    cp.impulse_j = Vec3(0, 0, 0);
    cp.separation_i = Vec3(0, 0, 0);
    cp.separation_j = Vec3(0, 0, 0);
    return cp;
  }

  double restitution = 1.0;
  double j = -(1 + restitution) * velocityAlongNormal;
  j /= (1 / p1.mass + 1 / p2.mass);

  Vec3 impulse = normal * j;
  cp.impulse_i = impulse * (1 / p1.mass);
  cp.impulse_j = impulse * (-1 / p2.mass);

  double overlap = (p1.radius + p2.radius) - dist;
  if (overlap > 0) {
    Vec3 separation = normal * (overlap / 2);
    cp.separation_i = separation;
    cp.separation_j = separation * (-1.0);
  } else {
    cp.separation_i = Vec3(0, 0, 0);
    cp.separation_j = Vec3(0, 0, 0);
  }

  return cp;
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

  {
    std::vector<std::vector<CollisionPair>> local_collisions(num_threads);
    std::atomic<size_t> current_row{0};

    for (size_t t = 0; t < num_threads; ++t) {
      threads.emplace_back([this, t, &current_row, &local_collisions]() {
        while (true) {
          size_t i = current_row.fetch_add(1, std::memory_order_relaxed);
          if (i >= spheresAmount) break;

          for (size_t j = i + 1; j < spheresAmount; ++j) {
            if (checkCollision(spheres[i], spheres[j])) {
              // Вычисляем импульсы, но не применяем их
              CollisionPair cp = computeCollisionResponse(spheres[i], spheres[j], i, j);
              local_collisions[t].push_back(cp);
            }
          }
        }
      });
    }

    for (auto& th : threads) th.join();
    threads.clear();

    // Собираем все коллизии
    collision_buffer.clear();
    for (auto& local : local_collisions) {
      collision_buffer.insert(collision_buffer.end(), local.begin(), local.end());
    }
  }

  {
    // Вариант A: Атомарные операции (медленнее, но проще)
    std::vector<Vec3> velocity_deltas(spheresAmount, Vec3(0, 0, 0));
    std::vector<Vec3> position_deltas(spheresAmount, Vec3(0, 0, 0));
    std::vector<std::atomic<int>> counters(spheresAmount);

    for (auto& c : counters) c.store(0);

    size_t chunk_size = (collision_buffer.size() + num_threads - 1) / num_threads;

    for (size_t t = 0; t < num_threads; ++t) {
      threads.emplace_back([this, t, chunk_size, &velocity_deltas, &position_deltas, &counters]() {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, collision_buffer.size());

        for (size_t k = start; k < end; ++k) {
          const auto& cp = collision_buffer[k];

          // Атомарно накапливаем изменения
          // (Это упрощение - для настоящей реализации нужны атомарные Vec3 или мьютексы)
          velocity_deltas[cp.i] += cp.impulse_i;
          velocity_deltas[cp.j] += cp.impulse_j;
          position_deltas[cp.i] += cp.separation_i;
          position_deltas[cp.j] += cp.separation_j;
          counters[cp.i].fetch_add(1, std::memory_order_relaxed);
          counters[cp.j].fetch_add(1, std::memory_order_relaxed);
        }
      });
    }

    for (auto& th : threads) th.join();
    threads.clear();

    // Применяем усреднённые изменения
    chunk_size = (spheresAmount + num_threads - 1) / num_threads;
    for (size_t t = 0; t < num_threads; ++t) {
      threads.emplace_back([this, t, chunk_size, &velocity_deltas, &position_deltas, &counters]() {
        size_t start = t * chunk_size;
        size_t end = std::min(start + chunk_size, spheresAmount);

        for (size_t i = start; i < end; ++i) {
          int count = counters[i].load(std::memory_order_relaxed);
          if (count > 0) {
            // Усредняем импульсы от множественных коллизий
            spheres[i].vel += velocity_deltas[i] / static_cast<double>(count);
            spheres[i].pos += position_deltas[i] / static_cast<double>(count);
          }
        }
      });
    }

    for (auto& th : threads) th.join();
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
