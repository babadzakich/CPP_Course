#pragma once

#include <3DEngine/BaseEngine.hpp>
#include <3DEngine/Utility/SIMDParticle.hpp>

class SIMDEngine : public BaseEngine {
    std::vector<SIMDParticle> SIMDspheres;

    void resolveCollisions(SIMDParticle& a, SIMDParticle& b);
  bool checkCollision(const SIMDParticle& a, const SIMDParticle& b);

    public:
    SIMDEngine(size_t amount) : BaseEngine("simd", amount) {}
    ~SIMDEngine() override = default;

    void init_from_text(const std::string& filename) override;
    void save_to_text(const std::string& filename) override;
    void step(double dt) override;
    std::vector<Particle> snapshot() const override;
    ConservationValues compute_conserv() const override;
};
