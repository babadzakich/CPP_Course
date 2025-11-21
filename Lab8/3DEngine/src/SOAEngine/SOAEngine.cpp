#include <3DEngine/SOAEngine/SOAEngine.hpp>

#include <fstream>

void SOAEngine::init_from_text(const std::string& filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for reading: " + filename);
  }
  spheres.clear();

  file.read(reinterpret_cast<char*>(&spheresAmount), sizeof(spheresAmount));

  spheres.resize(spheresAmount);
  file.read(reinterpret_cast<char*>(spheres.data()), spheresAmount * sizeof(Particle));
  x.resize(spheresAmount); y.resize(spheresAmount); z.resize(spheresAmount);
        vx.resize(spheresAmount); vy.resize(spheresAmount); vz.resize(spheresAmount);
        mass.resize(spheresAmount); radius.resize(spheresAmount);
        inv_mass.resize(spheresAmount);

    for (size_t i = 0; i < spheresAmount; i++) {
        x[i] = spheres[i].pos.x;
        y[i] = spheres[i].pos.y;
        z[i] = spheres[i].pos.z;
        vx[i] = spheres[i].vel.x;
        vy[i] = spheres[i].vel.y;
        vz[i] = spheres[i].vel.z;
        mass[i] = spheres[i].mass;
        radius[i] = spheres[i].radius;
        inv_mass[i] = 1.0 / mass[i];
    }
}

void SOAEngine::save_to_text(const std::string& filename) {
        // Конвертация обратно в AoS для сохранения
        for (size_t i = 0; i < spheresAmount; i++) {
            spheres[i].pos.x = x[i];
            spheres[i].pos.y = y[i];
            spheres[i].pos.z = z[i];
            spheres[i].vel.x = vx[i];
            spheres[i].vel.y = vy[i];
            spheres[i].vel.z = vz[i];
        }

        std::ofstream file(filename, std::ios::binary);
        file.write(reinterpret_cast<const char*>(&spheresAmount), sizeof(spheresAmount));
        file.write(reinterpret_cast<const char*>(spheres.data()), spheresAmount * sizeof(Particle));
}

void SOAEngine::step(double dt) {
    for (size_t i = 0; i < spheresAmount; i++) {
        x[i] += vx[i] * dt;
        y[i] += vy[i] * dt;
        z[i] += vz[i] * dt;
    }

    for (size_t i = 0; i < spheresAmount; i++) {
        for (size_t j = i + 1; j < spheresAmount; j++) {
            if (checkCollision(i,j)) {
                resolveCollisions(i,j);
            }
        }
    }
}

bool SOAEngine::checkCollision(const size_t& i, const size_t& j) {
  Vec3 delta(x[i] - x[j], y[i] - y[j], z[i] - z[j]);
  double distSquared = Vec3Util::lengthSq(delta);
  double radiusSum = radius[i] + radius[j];
  return distSquared <= radiusSum * radiusSum;
}

void SOAEngine::resolveCollisions(size_t& i, size_t& j) {
    // Zero-copy оптимизация: работаем напрямую с индексами и SOA массивами
    Vec3 pos_i(x[i], y[i], z[i]);
    Vec3 pos_j(x[j], y[j], z[j]);
    Vec3 vel_i(vx[i], vy[i], vz[i]);
    Vec3 vel_j(vx[j], vy[j], vz[j]);

    Vec3 normal = Vec3Util::normalize(pos_i - pos_j);
    double mp1 = inv_mass[i], mp2 = inv_mass[j];

    // Относительная скорость
    Vec3 relativevel = vel_i - vel_j;

    // Скорость сближения вдоль нормали
    double velocityAlongNormal = Vec3Util::dot(relativevel, normal);

    // Если сферы расходятся, столкновения нет
    if (velocityAlongNormal > 0) {
      return;
    }

    // Коэффициент восстановления (1.0 для абсолютно упругого столкновения)
    double restitution = 1.0;

    // Импульс столкновения
    double impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= (mp1 + mp2);

    // Применяем импульс
    Vec3 impulse = normal * impulseScalar;
    vel_i = vel_i + impulse * mp1;
    vel_j = vel_j - impulse * mp2;

    // Разделяем сферы, если они пересекаются
    Vec3 diff = pos_i - pos_j;
    double distance = Vec3Util::length(diff);
    double overlap = (radius[i] + radius[j]) - distance;

    if (overlap > 0) {
      Vec3 separation = normal * (overlap / 2);
      pos_i = pos_i + separation;
      pos_j = pos_j - separation;
    }

    // Записываем обновленные значения обратно в SOA массивы
    x[i] = pos_i.x; y[i] = pos_i.y; z[i] = pos_i.z;
    vx[i] = vel_i.x; vy[i] = vel_i.y; vz[i] = vel_i.z;

    x[j] = pos_j.x; y[j] = pos_j.y; z[j] = pos_j.z;
    vx[j] = vel_j.x; vy[j] = vel_j.y; vz[j] = vel_j.z;
}

std::vector<Particle> SOAEngine::snapshot() const {
    std::vector<Particle> snapshot(spheresAmount);
    for (size_t i = 0; i < spheresAmount; i++) {
        snapshot[i] = Particle(
            spheres[i].id,
            mass[i],
            radius[i],
            Vec3(x[i], y[i], z[i]),
            Vec3(vx[i], vy[i], vz[i])
        );
    }
    return snapshot;
}

ConservationValues SOAEngine::compute_conserv() const {
  ConservationValues cv;
  Vec3 total_momentum(0, 0, 0);

  for (size_t i = 0; i < spheresAmount; i++) {
    Vec3 vel(vx[i], vy[i], vz[i]);
    total_momentum += vel * mass[i];
    cv.total_kinetic_energy += 0.5 * mass[i] * Vec3Util::lengthSq(vel);
  }

  cv.total_momentum_magnitude = Vec3Util::length(total_momentum);
  return cv;
}
