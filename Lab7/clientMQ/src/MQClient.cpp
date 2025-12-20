#include <MQClient.hpp>

MQClient::MQClient(const std::string& broker_address) : socket(context, zmq::socket_type::dealer) {
  socket.connect(broker_address);
}

MQClient::~MQClient() {
  socket.close();
  context.close();
}

void MQClient::publish(const std::string& queue, const std::string& message, int qos) {
  json j = {{"action", "publish"}, {"queue", queue}, {"message", message}, {"qos", qos}};
  json response = sendRequest(j);
  if (qos == 1 && response.contains("message_id")) {
    spdlog::info("Published message to queue '{}' with message_id '{}'", queue,
                 response["message_id"].get<std::string>());
  } else {
    spdlog::info("Published message to queue '{}' with QoS={}", queue, qos);
  }
}

void MQClient::subscribe(const std::string& queue, int qos, int ack_timeout) {
  json j = {{"action", "subscribe"}, {"queue", queue}, {"qos", qos}, {"ack_timeout", ack_timeout}};
  json response = sendRequest(j);
  spdlog::info("Subscribed to queue '{}' with QoS={} and ack_timeout={}", queue, qos, ack_timeout);
}

void MQClient::unsubscribe(const std::string& queue) {
  json j = {{"action", "unsubscribe"}, {"queue", queue}};
  json response = sendRequest(j);
  spdlog::info("Unsubscribed from queue '{}'", queue);
}

void MQClient::ack(const std::string& message_id) {
  json j = {{"action", "ack"}, {"message_id", message_id}};
  json response = sendRequest(j);
  spdlog::debug("Acknowledged message with message_id '{}'", message_id);
}

std::string MQClient::poll() {
  json j = {{"action", "poll"}};
  json response = sendRequest(j);
  if (response.contains("message") && !response["message"].is_null()) {
    std::string message = response["message"].get<std::string>();
    if (response.contains("message_id")) {
      std::string message_id = response["message_id"].get<std::string>();
      spdlog::info("Received message with message_id '{}' : {}", message_id, message);
    } else {
      spdlog::info("Received message: {}", message);
    }
    spdlog::info("Received message from broker");
    return message;
  } else {
    spdlog::info("No message available from broker");
    return "";
  }
}

json MQClient::sendRequest(const json& j) {
  std::string request_str = j.dump();

  zmq::message_t request(request_str.begin(), request_str.end());
  socket.send(request, zmq::send_flags::none);

  zmq::message_t reply;
  auto recv_result = socket.recv(reply, zmq::recv_flags::none);
  if (!recv_result) {
    spdlog::error("Failed to receive reply from broker");
    return {{"status", "error"}, {"message", "No reply from broker"}};
  }

  std::string reply_str(static_cast<char*>(reply.data()), reply.size());
  return json::parse(reply_str);
}