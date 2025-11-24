#pragma once

#include <3DEngine/BaseEngine.hpp>

#include <unordered_map>
#include <set>

class SpatHashEngine : public BaseEngine {
    double cell_size;
    std::unordered_map<int64_t, std::vector<size_t>> grid;
    // std::set<std::pair<size_t, size_t>> checked_pairs;

    int64_t hash_coord(int x, int y, int z) const {
      uint64_t ux = static_cast<uint64_t>(x) & 0xFFFFF;
      uint64_t uy = static_cast<uint64_t>(y) & 0xFFFFF;
      uint64_t uz = static_cast<uint64_t>(z) & 0xFFFFF;

      return static_cast<int64_t>((ux << 40) | (uy << 20) | uz);
    }

    int64_t get_cell(double x, double y, double z) const {
        int cx = int(std::floor(x / cell_size));
        int cy = int(std::floor(y / cell_size));
        int cz = int(std::floor(z / cell_size));
        return hash_coord(cx, cy, cz);
    }
    std::tuple<int, int, int> get_cell_coords(int64_t hash) const {
      uint64_t uhash = static_cast<uint64_t>(hash);
      int x = static_cast<int>((uhash >> 40) & 0xFFFFF);
      int y = static_cast<int>((uhash >> 20) & 0xFFFFF);
      int z = static_cast<int>(uhash & 0xFFFFF);
      return {x, y, z};
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
