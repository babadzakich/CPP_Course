#pragma once
#include <3DEngine/BaseEngine.hpp>

class SimpleEngine : public BaseEngine {
  void resolveCollisions(Particle& a, Particle& b);
  bool checkCollision(const Particle& a, const Particle& b);

 public:
 SimpleEngine(size_t amount) : BaseEngine("simple", amount) {}
  void init_from_text(const std::string& filename) override;
  void save_to_text(const std::string& filename) override;
  void step(double dt) override;
  std::vector<Particle> snapshot() const override;
  ConservationValues compute_conserv() const override;
};
