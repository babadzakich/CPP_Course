#pragma once

#include <3DEngine/BaseEngine.hpp>

class BinaryEngine : public BaseEngine {

  Particle* spheres;

    int mmap_fd = -1;
    void* mmap_addr = nullptr;
    size_t mmap_size = 0;

  std::vector<double> precalcMass;
  void resolveCollisions(Particle& a, Particle& b);
  bool checkCollision(const Particle& a, const Particle& b);

 public:
  BinaryEngine(size_t amount) : BaseEngine("binary", amount) {}
  void init_from_text(const std::string& filename) override;
  void unmap_file();
  void save_to_text(const std::string& filename) override;
  void step(double dt) override;
  std::vector<Particle> snapshot() const override;
  ConservationValues compute_conserv() const override;
};
