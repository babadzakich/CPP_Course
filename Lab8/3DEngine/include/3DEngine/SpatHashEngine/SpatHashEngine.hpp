#pragma once

#include <3DEngine/BaseEngine.hpp>

#include <unordered_map>

class SpatHashEngine : public BaseEngine {
    double cell_size;
    std::unordered_map<int64_t, std::vector<size_t>> grid;

    int64_t hash_coord(int x, int y, int z) const {
        return (int64_t(x) << 40) | (int64_t(y) << 20) | int64_t(z);
    }

    int64_t get_cell(double x, double y, double z) const {
        int cx = int(std::floor(x / cell_size));
        int cy = int(std::floor(y / cell_size));
        int cz = int(std::floor(z / cell_size));
        return hash_coord(cx, cy, cz);
    }

  void resolveCollisions(Particle& a, Particle& b);
  bool checkCollision(const Particle& a, const Particle& b);

public:
  SpatHashEngine(size_t amount) : BaseEngine("spat_hash", amount) {}
  void init_from_text(const std::string& filename) override;
  void save_to_text(const std::string& filename) override;
  void step(double dt) override;
  std::vector<Particle> snapshot() const override;
  ConservationValues compute_conserv() const override;
};
