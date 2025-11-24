#pragma once

#include <3DEngine/BaseEngine.hpp>
#include <thread>
#include <vector>
#include <condition_variable>

class MultithreadEngine : public BaseEngine {
    size_t num_threads;

    void resolveCollisions(Particle& a, Particle& b);
    bool checkCollision(const Particle& a, const Particle& b);

 public:
    MultithreadEngine(size_t amount, size_t threads = 0) : BaseEngine("multithread", amount), num_threads(threads == 0 ? std::thread::hardware_concurrency() : threads) {}

    void init_from_text(const std::string& filename) override;
    void save_to_text(const std::string& filename) override;
    void step(double dt) override;
    std::vector<Particle> snapshot() const override;
    ConservationValues compute_conserv() const override;
};
