#include <3DEngine/SimpleEngine/SimpleEngine.hpp>
#include <3DEngine/BinaryEngine/BinaryEngine.hpp>
#include <3DEngine/SpatHashEngine/SpatHashEngine.hpp>
#include <3DEngine/SOAEngine/SOAEngine.hpp>
#include <3DEngine/MultithreadEngine/MultithreadEngine.hpp>

#include <iostream>
#include <memory>
#include <chrono>
#include <iomanip>
#include <random>
#include <fstream>

#include "CLI/CLI.hpp"

std::unique_ptr<BaseEngine> create_engine(const std::string& engine_type, const size_t amount, const size_t threads) {
  if (engine_type == "simple") {
    return std::make_unique<SimpleEngine>(amount);
  } else if (engine_type == "binary") {
    return std::make_unique<BinaryEngine>(amount);
  } else if (engine_type == "spat_hash") {
    return std::make_unique<SpatHashEngine>(amount);
  } else if (engine_type == "soa") {
    return std::make_unique<SOAEngine>(amount);
  } else if (engine_type == "multithread") {
    return std::make_unique<MultithreadEngine>(amount, threads);
  } else {
    throw std::invalid_argument("Unknown engine type: " + engine_type);
  }
}

// Benchmark engine
struct BenchResult {
  std::string name;
  Metrics metrics;
};

BenchResult benchmark_engine(std::unique_ptr<BaseEngine> engine, const std::string& name,
                            const std::string& test_file, size_t warmup_steps, size_t work_steps
                            , const double dt) {
  BenchResult result{name, {}};

  // Init timing
  auto start = std::chrono::high_resolution_clock::now();
  engine->init_from_text(test_file);
  auto end = std::chrono::high_resolution_clock::now();
  double init_time = std::chrono::duration<double>(end - start).count();
  result.metrics.add_init(init_time);

  // std::cout << "Test stuff" << std::endl;
  // for (auto p : engine->snapshot()) {
  //   std::cout << p.id << " " << p.pos.x << " " << p.pos.y
  //   << p.pos.z << " " << p.vel.x << " " << p.vel.y << " " << p.vel.z
  //   << " " << p.mass << " " << p.radius << "\n";
  // }

  std::cout << "--Init Conserv Values--" << std::endl;
  auto conserv_start = engine->compute_conserv();
  std::cout << "  Energy: " << conserv_start.total_kinetic_energy << " J\n";
  std::cout << "  Momentum magnitude: " << conserv_start.total_momentum_magnitude << " kg*m/s\n";

  // Warmup
  for (size_t i = 0; i < warmup_steps; ++i) {
    start = std::chrono::high_resolution_clock::now();
    engine->step(dt);
    end = std::chrono::high_resolution_clock::now();
    double step_time = std::chrono::duration<double>(end - start).count();
    result.metrics.add_warmup(step_time);
  }

  // Work phase
  for (size_t i = 0; i < work_steps; ++i) {
    start = std::chrono::high_resolution_clock::now();
    engine->step(dt);
    end = std::chrono::high_resolution_clock::now();
    double step_time = std::chrono::duration<double>(end - start).count();
    result.metrics.add_work(step_time);
  }

  // Save timing
  std::string output_file = "output_" + name + (name == "simple" ? ".txt" : ".bin");
  start = std::chrono::high_resolution_clock::now();
  engine->save_to_text(output_file);
  end = std::chrono::high_resolution_clock::now();
  double save_time = std::chrono::duration<double>(end - start).count();
  result.metrics.add_save(save_time);

  // Validation
  std::cout << "--Final Conserv Values--" << std::endl;
  auto conserv = engine->compute_conserv();
  std::cout << "  Energy: " << conserv.total_kinetic_energy << " J\n";
  std::cout << "  Momentum magnitude: " << conserv.total_momentum_magnitude << " kg*m/s\n";

  return result;
}

int main(int argc, char* argv[]) {
  CLI::App app{"3D Engine Benchmarking Tool"};

  std::string engine_type;
  app.add_option("-e, --engine", engine_type, "Type of engine to benchmark (simple, binary, spat_hash, soa, multithread)")->required();

  size_t threads = std::thread::hardware_concurrency();
  app.add_option("-t, --threads", threads, "Number of threads for multithreaded engine")->default_val(threads);

  std::string test_data_file;
  app.add_option("-f, --file", test_data_file, "Test data file")->required();

  size_t particle_count;
  app.add_option("-p, --particles", particle_count, "Number of particles")->required();

  size_t warmup_steps = 10;
  app.add_option("-w, --warmup", warmup_steps, "Number of warmup steps")->default_val(warmup_steps);

  size_t work_steps = 100;
  app.add_option("-W, --work", work_steps, "Number of work steps")->default_val(work_steps);

  double dt = 0.016;
  app.add_option("-d, --dt", dt, "Time step for each simulation step")->default_val(dt);

  CLI11_PARSE(app, argc, argv);

  if (engine_type == "simple" && test_data_file.contains(".bin")) {
    throw std::invalid_argument("SimpleEngine does not support binary files.");
  }

  BenchResult results;


  std::cout << "\n> Benchmarking " << engine_type << " Engine\n";
  try {
    results = benchmark_engine(create_engine(engine_type, particle_count, threads), engine_type, test_data_file, warmup_steps, work_steps, dt);
  } catch (const std::exception& e) {
    std::cout << "  ERROR: " << e.what() << "\n";
  }

  results.metrics.print(results.name);


  return 0;
}
