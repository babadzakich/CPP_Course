#include <3DEngine/BinaryEngine/BinaryEngine.hpp>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <type_traits>
#include <fstream>

void BinaryEngine::init_from_text(const std::string& filename) {

    unmap_file();

    mmap_fd = open(filename.c_str(), O_RDONLY);
    if (mmap_fd < 0) throw std::runtime_error("open failed");

    struct stat st;
    if (fstat(mmap_fd, &st) != 0) { close(mmap_fd); mmap_fd = -1; throw std::runtime_error("fstat failed"); }
    mmap_size = static_cast<size_t>(st.st_size);

    if (mmap_size < sizeof(u_int64_t)) { close(mmap_fd); mmap_fd = -1; throw std::runtime_error("file too small"); }

    // Здесь предполагаем, что файл - просто последовательность Particle (без заголовка)
    mmap_addr = mmap(nullptr, mmap_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, mmap_fd, 0);
    if (mmap_addr == MAP_FAILED) { mmap_addr = nullptr; close(mmap_fd); mmap_fd = -1; throw std::runtime_error("mmap failed"); }


    // Убедимся что размер кратен sizeof(Particle)
    size_t expected = spheresAmount * sizeof(Particle);

    if (mmap_size < expected) {
        unmap_file();
        throw std::runtime_error("file size is not multiple of Particle size");
    }

    spheres = reinterpret_cast<Particle*>(mmap_addr);
    // precalcMass: вычисляем в векторе
    precalcMass.resize(spheresAmount);
    for (size_t i = 0; i < spheresAmount; ++i) {
        // безопасно обращаться к spheres[i] — это mapped memory (read-only), но mass читаем
        precalcMass[i] = 1.0 / spheres[i].mass;
    }
}

void BinaryEngine::unmap_file() {
    if (mmap_addr) {
        munmap(mmap_addr, mmap_size);
        mmap_addr = nullptr;
    }
    if (mmap_fd >= 0) {
        close(mmap_fd);
        mmap_fd = -1;
    }
    mmap_size = 0;
    spheres = nullptr;
    spheresAmount = 0;
    precalcMass.clear();
}

void BinaryEngine::save_to_text(const std::string& filename) {
  std::ofstream file(filename, std::ios::binary);

  file.write(reinterpret_cast<const char*>(spheres), spheresAmount * sizeof(Particle));
}

void BinaryEngine::step(double dt) {
  for (size_t i = 0; i < spheresAmount; i++)
    spheres[i].pos += spheres[i].vel * dt;
  for (size_t i = 0; i < spheresAmount; i++) {
    for (size_t j = i + 1; j < spheresAmount; j++) {
      if (checkCollision(spheres[i], spheres[j]))
        resolveCollisions(spheres[i], spheres[j]);
    }
  }
}

bool BinaryEngine::checkCollision(const Particle& p1, const Particle& p2) {
  Vec3 delta = p1.pos - p2.pos;
  double distSquared = Vec3Util::lengthSq(delta);
  double radiusSum = p1.radius + p2.radius;
  return distSquared <= radiusSum * radiusSum;
}

void BinaryEngine::resolveCollisions(Particle& p1, Particle& p2) {
  // Вектор от центра p2 к центру p1
  Vec3 normal = Vec3Util::normalize(p1.pos - p2.pos);
  double mp1 = precalcMass[p1.id], mp2 = precalcMass[p2.id];
  // Относительная скорость
  Vec3 relativevel = p1.vel - p2.vel;

  // Скорость сближения вдоль нормали
  double velocityAlongNormal = Vec3Util::dot(relativevel, normal);

  // Если сферы расходятся, столкновения нет
  if (velocityAlongNormal > 0) {
    return;
  }

  // Коэффициент восстановления (1.0 для абсолютно упругого столкновения)
  double restitution = 1.0;

  // Импульс столкновения
  double j = -(1 + restitution) * velocityAlongNormal;
  j /= (mp1 + mp2);

  // Применяем импульс
  Vec3 impulse = normal * j;
  p1.vel = p1.vel + impulse * (mp1);
  p2.vel = p2.vel - impulse * (mp2);

  // Разделяем сферы, если они пересекаются
  Vec3 diff = p1.pos - p2.pos;
  double distance = Vec3Util::length(diff);
  double overlap = (p1.radius + p2.radius) - distance;

  if (overlap > 0) {
    Vec3 separation = normal * (overlap / 2);
    p1.pos = p1.pos + separation;
    p2.pos = p2.pos - separation;
  }
}

std::vector<Particle> BinaryEngine::snapshot() const {
  return std::vector<Particle>(spheres, spheres + spheresAmount);
}

ConservationValues BinaryEngine::compute_conserv() const {
  ConservationValues cv;
  Vec3 total_momentum(0, 0, 0);

  for (size_t i = 0; i < spheresAmount; i++) {
    total_momentum += spheres[i].vel * spheres[i].mass;
    cv.total_kinetic_energy += 0.5 * spheres[i].mass * Vec3Util::lengthSq(spheres[i].vel);
  }

  cv.total_momentum_magnitude = Vec3Util::length(total_momentum);
  return cv;
}
