#pragma once
#include <3DEngine/Utility/Metriks.hpp>
#include <3DEngine/Utility/Particle.hpp>
#include <3DEngine/Utility/Vec3.hpp>
#include <string>
#include <vector>

class BaseEngine {
 protected:
  std::string name = "base";
  size_t spheresAmount;
  std::vector<Particle> spheres;
  double total_energy = 0;
  double total_momentum_x = 0, total_momentum_y = 0, total_momentum_z = 0;

  void calculate_conservation() {
      total_energy = 0;
      total_momentum_x = total_momentum_y = total_momentum_z = 0;

      for (const auto& b : spheres) {
          double v2 = b.vel.x*b.vel.x + b.vel.y*b.vel.y + b.vel.z*b.vel.z;
          total_energy += 0.5 * b.mass * v2;
          total_momentum_x += b.mass * b.vel.x;
          total_momentum_y += b.mass * b.vel.y;
          total_momentum_z += b.mass * b.vel.z;
      }
  }

 public:
  BaseEngine(const std::string& n, size_t amount) : name(n), spheresAmount(amount) {}
  virtual ~BaseEngine() = default;

  virtual void init_from_text(const std::string& filename) = 0;
  virtual void save_to_text(const std::string& filename) = 0;
  virtual void step(double dt) = 0;
  virtual std::vector<Particle> snapshot() const = 0;
  virtual ConservationValues compute_conserv() const = 0  ;
};
