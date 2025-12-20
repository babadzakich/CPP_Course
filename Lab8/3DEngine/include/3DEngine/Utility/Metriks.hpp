#pragma once
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <fstream>
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
    std::string filename = name + "_benchmark_results.txt";
    std::ofstream output(filename);
    if (!output.is_open()) throw std::runtime_error("couldn`t open benchmark reuslts file " + filename);

    output << std::fixed << std::setprecision(10);

      output << "---INIT TIME---" << '\n';
    for (auto& time : init_times)
      output << time << '\n';

      output << "---WARMUP TIME---" << '\n';
    for (auto& time : warmup_times)
      output << time << '\n';

      output << "---WORK TIME---" << '\n';
    for (auto& time : work_times)
      output << time << '\n';

      output << "---SAVE TIME---" << '\n';
    for (auto& time : save_times)
      output << time << '\n';
  }
};
