#include <spdlog/spdlog.h>
#include <BabadzakiMQ/Brocker/BabadzakiMQ.hpp>
#include <BabadzakiMQ/Configuration/ConfigReader.hpp>

#include <csignal>
#include <iostream>

std::atomic<bool> shutdown_requested{false};

void signalHandler(int signal) {
  spdlog::info("Received signal {}, shutting down...", signal);
  shutdown_requested = true;
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <config_file.yaml>" << std::endl;
    return EXIT_FAILURE;
  }

  spdlog::set_level(spdlog::level::info);

  try {
    ConfigReader reader(argv[1]);
    std::vector<Config> queues_config = reader.readQueues();

    spdlog::info("Loaded {} queue(s) from config", queues_config.size());
    for (const auto& config : queues_config) {
      spdlog::info("  - Queue: {}, TTL: {}s", config.name, config.message_ttl);
    }

    std::string bind_address = "tcp://*:5555";
    BabadzakiMQ broker(queues_config, bind_address);

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    spdlog::info("Broker is running. Press Ctrl+C to stop.");

    std::thread broker_thread([&broker]() { broker.loop(); });

    while (!shutdown_requested) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    broker.stop();

    if (broker_thread.joinable()) {
      broker_thread.join();
    }

    spdlog::info("Broker stopped successfully");

  } catch (const std::exception& e) {
    spdlog::error("Error: {}", e.what());
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}