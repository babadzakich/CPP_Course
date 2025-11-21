#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <vector>
#include <3DEngine/Utility/Vec3.hpp>
#include <3DEngine/Utility/Particle.hpp>

struct ConservationValues {
  double total_momentum_magnitude;
  double total_kinetic_energy;

  ConservationValues() : total_momentum_magnitude(0), total_kinetic_energy(0) {}
};

struct Metrics {
  std::vector<double> init_times;
  std::vector<double> warmup_times;
  std::vector<double> work_times;
  std::vector<double> save_times;

  void add_init(double t) { init_times.push_back(t); }
  void add_warmup(double t) { warmup_times.push_back(t); }
  void add_work(double t) { work_times.push_back(t); }
  void add_save(double t) { save_times.push_back(t); }

  void print(const std::string& name) const {
    auto stats = [](const std::vector<double>& v) {
      if (v.empty())
        return std::string("N/A");
      double sum = std::accumulate(v.begin(), v.end(), 0.0);
      double avg = sum / v.size();
      double min = *std::min_element(v.begin(), v.end());
      double max = *std::max_element(v.begin(), v.end());
      double var = 0;
      for (auto x : v)
        var += (x - avg) * (x - avg);
      var /= v.size();

      std::ostringstream oss;
      oss << std::fixed << std::setprecision(3) << "avg=" << avg * 1000 << "ms min=" << min * 1000
          << "ms max=" << max * 1000 << "ms var=" << var * 1000000;
      return oss.str();
    };

    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Init: " << stats(init_times) << "\n";
    std::cout << "Warmup: " << stats(warmup_times) << "\n";
    std::cout << "Work: " << stats(work_times) << "\n";
    std::cout << "Save: " << stats(save_times) << "\n";
  }
};
