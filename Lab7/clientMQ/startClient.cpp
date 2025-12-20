#include <MQClient.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
  spdlog::set_level(spdlog::level::info);
  MQClient client("tcp://localhost:5555");

  client.subscribe("test_queue", 1, 30);
  client.publish("test_queue", "Hello, World!", 1);

  std::string response = client.poll();
  std::cout << "Received response: " << response << std::endl;

  return 0;
}