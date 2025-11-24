#include <iostream>
#include <random>
#include <fstream>
#include <chrono>

#include <3DEngine/Utility/Vec3.hpp>
#include <3DEngine/Utility/Particle.hpp>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << "<particles amount> <bin or simple> <filename>\n";
        return EXIT_FAILURE;
    }
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  std::uniform_real_distribution<double> pos_dist(-100.0, 100.0);
  std::uniform_real_distribution<double> vel_dist(-10.0, 10.0);
  std::uniform_real_distribution<double> mass_dist(1.0, 5.0);
  std::uniform_real_distribution<double> radius_dist(0.5, 2.0);
    const size_t particle_count = std::stoul(argv[1]);
    const bool binary = std::string(argv[2]) == "bin";
    std::string filename = std::to_string(particle_count) + std::string(argv[3]) + (binary ? ".bin" : ".txt");
  std::ofstream file(filename, binary ? std::ios::binary : std::ios::out);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to create test file");
  }

  for (size_t i = 0; i < particle_count; ++i) {
    Particle p;
    p.id = i;
    p.pos = Vec3(pos_dist(rng), pos_dist(rng), pos_dist(rng));
    p.vel = Vec3(vel_dist(rng), vel_dist(rng), vel_dist(rng));
    p.mass = mass_dist(rng);
    p.radius = radius_dist(rng);
    if (!binary) {
      file << i << " " << p.pos.x << " " << p.pos.y << " " << p.pos.z << " "
           << p.vel.x << " " << p.vel.y << " " << p.vel.z << " "
           << p.mass << " " << p.radius << "\n";
      continue;
    }
    file.write(reinterpret_cast<const char*>(&p), sizeof(Particle));
  }
}
