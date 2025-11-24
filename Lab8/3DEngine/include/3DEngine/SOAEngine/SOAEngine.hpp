#pragma once

#include <3DEngine/BaseEngine.hpp>

class SOAEngine : public BaseEngine {
    std::vector<double> x, y, z;
    std::vector<double> vx, vy, vz;
    std::vector<double> mass, radius;
    std::vector<double> inv_mass;

    void resolveCollisions(size_t a, size_t b);
    bool checkCollision(const size_t& a, const size_t& b);
public:
SOAEngine(size_t amount) : BaseEngine("soa", amount) {}
  void init_from_text(const std::string& filename) override;
  void save_to_text(const std::string& filename) override;
  void step(double dt) override;
  std::vector<Particle> snapshot() const override;
  ConservationValues compute_conserv() const override;
};
