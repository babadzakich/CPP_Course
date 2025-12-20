#pragma once

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <string>
#include <zmq.hpp>

using json = nlohmann::json;

class MQClient {
  zmq::context_t context{1};
  zmq::socket_t socket;

  json sendRequest(const json& j);

 public:
  MQClient(const std::string& broker_address);
  ~MQClient();

  void publish(const std::string& queue, const std::string& message, int qos = 0);
  void subscribe(const std::string& queue, int qos = 0, int ack_timeout = 30);
  void unsubscribe(const std::string& queue);
  void ack(const std::string& message_id);
  std::string poll();
};