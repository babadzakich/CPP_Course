#include <3DEngine/SpatHashEngine/SpatHashEngine.hpp>

#include <fstream>

void SpatHashEngine::init_from_text(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Couldn`t open config file");

    spheres.resize(spheresAmount);

    file.read(reinterpret_cast<char*>(spheres.data()), spheresAmount * sizeof(Particle));
    cell_size = 0;

    for(const auto& sphere: spheres) {
        cell_size = std::max(cell_size, sphere.radius);
    }

    cell_size *= 3.0;
}

void SpatHashEngine::save_to_text(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);

    file.write(reinterpret_cast<const char*>(spheres.data()), spheresAmount * sizeof(Particle));
}

void SpatHashEngine::step(double dt) {
    for (auto& s : spheres) {
        s.pos += s.vel * dt;
        s.pos = Vec3Util::wrap_pos(s.pos);
    }

    grid.clear();
    for (size_t i = 0; i < spheres.size(); ++i) {
        int64_t cell = get_cell(spheres[i].pos.x, spheres[i].pos.y, spheres[i].pos.z);
        grid[cell].push_back(i);
    }

    for (const auto& [cell_key, cell_bodies] : grid) {
        for (size_t i = 0; i < cell_bodies.size(); i++) {
            for (size_t j = i + 1; j < cell_bodies.size(); j++) {
                if (checkCollision(spheres[cell_bodies[i]], spheres[cell_bodies[j]]))
                    resolveCollisions(spheres[cell_bodies[i]], spheres[cell_bodies[j]]);
            }
        }

        int x = (cell_key >> 40) & 0xFFFFF;
        int y = (cell_key >> 20) & 0xFFFFF;
        int z = cell_key & 0xFFFFF;

        for (int dx = 0; dx <= 1; dx++) {
            for (int dy = 0; dy <= 1; dy++) {
                for (int dz = 0; dz <= 1; dz++) {
                    if (dx == 0 && dy == 0 && dz == 0) continue;

                    int64_t neighbor = hash_coord(x + dx, y + dy, z + dz);
                    auto it = grid.find(neighbor);
                    if (it != grid.end()) {
                        for (size_t i : cell_bodies) {
                            for (size_t j : it->second) {
                                if (checkCollision(spheres[i], spheres[j]))
                                    resolveCollisions(spheres[i], spheres[j]);
                            }
                        }
                    }
                }
            }
        }
    }
}

std::vector<Particle> SpatHashEngine::snapshot() const {
    return spheres;
}

ConservationValues SpatHashEngine::compute_conserv() const {
  ConservationValues cv;
  Vec3 total_momentum(0, 0, 0);

  for (const auto& p : spheres) {
    total_momentum += p.vel * p.mass;
    cv.total_kinetic_energy += 0.5 * p.mass * Vec3Util::lengthSq(p.vel);
  }

  cv.total_momentum_magnitude = Vec3Util::length(total_momentum);
  return cv;
}

bool SpatHashEngine::checkCollision(const Particle& p1, const Particle& p2) {
  Vec3 delta = Vec3Util::minimum_image_delta(p1.pos, p2.pos);
  double distSquared = Vec3Util::lengthSq(delta);
  double radiusSum = p1.radius + p2.radius;
  return distSquared <= radiusSum * radiusSum;
}

void SpatHashEngine::resolveCollisions(Particle& p1, Particle& p2) {
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
  j /= (1/p1.mass + 1/p2.mass);

  Vec3 impulse = normal * j;
  p1.vel = p1.vel + impulse * (1/p1.mass);
  p2.vel = p2.vel - impulse * (1/p2.mass);

  double overlap = (p1.radius + p2.radius) - dist;

  if (overlap > 0) {
    Vec3 separation = normal * (overlap / 2);
    p1.pos = p1.pos + separation;
    p2.pos = p2.pos - separation;
  }
}
