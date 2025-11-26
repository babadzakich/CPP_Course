#pragma once

#include <3DEngine/BaseEngine.hpp>
#include <thread>
#include <vector>
#include <condition_variable>

class MultithreadEngine : public BaseEngine {
    size_t num_threads;

    struct CollisionPair {
        size_t i, j;
        Vec3 impulse_i, impulse_j;
        Vec3 separation_i, separation_j;
    };

    std::vector<CollisionPair> collision_buffer;

    void resolveCollisions(Particle& a, Particle& b);
    bool checkCollision(const Particle& a, const Particle& b);
    CollisionPair computeCollisionResponse(const Particle& p1, const Particle& p2, size_t idx1, size_t idx2);

 public:
    MultithreadEngine(size_t amount, size_t threads = 0) : BaseEngine("multithread", amount), num_threads(threads == 0 ? std::thread::hardware_concurrency() : threads) {}

    void init_from_text(const std::string& filename) override;
    void save_to_text(const std::string& filename) override;
    void step(double dt) override;
    std::vector<Particle> snapshot() const override;
    ConservationValues compute_conserv() const override;
};
