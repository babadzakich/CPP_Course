#include <BabadzakiMQ/Brocker/BabadzakiMQ.hpp>

std::string BabadzakiMQ::generateMessageId() {
  static std::random_device rd;
  static std::mt19937_64 gen(rd());
  static std::uniform_int_distribution<uint64_t> dis;

  std::ostringstream oss;
  oss << std::hex << std::setfill('0') << std::setw(16) << dis(gen);
  return oss.str();
}

BabadzakiMQ::BabadzakiMQ(const std::vector<Config>& configs, const std::string& bind_address)
    : socket(context, zmq::socket_type::router) {
  for (const auto& config : configs) {
    if (config.durability == Durability::Memory) {
      if (config.ordering == Ordering::None) {
        queues.push_back(std::make_unique<MemoryQueueNone>(config.name, std::chrono::seconds(config.message_ttl)));
      } else if (config.ordering == Ordering::FIFO) {
        queues.push_back(std::make_unique<MemoryQueueFIFO>(config.name, std::chrono::seconds(config.message_ttl)));
      }
    } else if (config.durability == Durability::Disk) {
      if (config.ordering == Ordering::None) {
        queues.push_back(std::make_unique<DiskQueueNone>(config.name, std::chrono::seconds(config.message_ttl)));
      } else if (config.ordering == Ordering::FIFO) {
        queues.push_back(std::make_unique<DiskQueueFIFO>(config.name, std::chrono::seconds(config.message_ttl)));
      }
    }
  }

  socket.bind(bind_address);
  spdlog::info("BabadzakiMQ broker started on {}", bind_address);

  cleanup_thread = std::thread(&BabadzakiMQ::cleanupLoop, this);
  timeout_thread = std::thread(&BabadzakiMQ::timeoutCheckLoop, this);
}

BabadzakiMQ::~BabadzakiMQ() {
  stop();
  if (cleanup_thread.joinable()) {
    cleanup_thread.join();
  }
  if (timeout_thread.joinable()) {
    timeout_thread.join();
  }

  socket.close();
  context.close();
}

void BabadzakiMQ::stop() {
  running = false;
}

IQueue* BabadzakiMQ::findQueue(const std::string& name) {
  for (auto& queue : queues) {
    if (queue->getName() == name) {
      return queue.get();
    }
  }
  return nullptr;
}

json BabadzakiMQ::handlePublish(const std::string& client_id, const std::string& queue_name, const std::string& message,
                                QoS qos) {
  auto* queue = findQueue(queue_name);
  if (!queue) {
    spdlog::warn("Queue '{}' not found", queue_name);
    return {{"status", "error"}, {"message", "Queue not found"}};
  }

  Message msg;
  msg.data = message;
  msg.timestamp = std::chrono::system_clock::now();
  queue->push(msg);
  spdlog::debug("Message published to queue '{}' from client '{}' with QoS={}", queue_name, client_id,
                static_cast<int>(qos));

  if (qos == QoS::WithAck) {
    std::string msg_id = generateMessageId();
    return {{{"status", "ok"}, {"action", "publish"}, {"message_id", msg_id}}};
  } else {
    return {};
  }
}

json BabadzakiMQ::handleSubscribe(const std::string& client_id, const std::string& queue_name, QoS qos,
                                  int ack_timeout_sec) {
  std::lock_guard<std::mutex> lock(clients_mutex);

  auto* queue = findQueue(queue_name);
  if (!queue) {
    spdlog::warn("Queue '{}' not found for subscription", queue_name);
    return {{"status", "error"}, {"message", "Queue not found"}};
  }

  auto& client = clients[client_id];
  client.identity = client_id;

  QueueSubscription sub;
  sub.queue_name = queue_name;
  sub.qos = qos;
  sub.ack_timeout_sec = ack_timeout_sec;
  sub.read_position = 0;

  client.subscriptions[queue_name] = sub;

  queue_subscribers[queue_name].push_back(client_id);

  spdlog::info("Client '{}' subscribed to queue '{}' with QoS={}, timeout={}s", client_id, queue_name,
               static_cast<int>(qos), ack_timeout_sec);

  return {{{"status", "ok"}, {"action", "subscribe"}, {"queue", queue_name}}};
}

json BabadzakiMQ::handleUnsubscribe(const std::string& client_id, const std::string& queue_name) {
  std::lock_guard<std::mutex> lock(clients_mutex);

  auto client_it = clients.find(client_id);
  if (client_it != clients.end()) {
    client_it->second.subscriptions.erase(queue_name);
  }

  auto& subscribers = queue_subscribers[queue_name];
  subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), client_id), subscribers.end());

  spdlog::info("Client '{}' unsubscribed from queue '{}'", client_id, queue_name);
  return {{{"status", "ok"}, {"action", "unsubscribe"}, {"queue", queue_name}}};
}

json BabadzakiMQ::handleAck(const std::string& client_id, const std::string& message_id) {
  std::lock_guard<std::mutex> lock(clients_mutex);

  auto client_it = clients.find(client_id);
  if (client_it == clients.end()) {
    spdlog::warn("Unknown client '{}' tried to ack", client_id);
    return {{"status", "error"}, {"message", "Client not registered"}};
  }

  auto& client = client_it->second;
  auto msg_it = client.pending_acks.find(message_id);
  if (msg_it == client.pending_acks.end()) {
    spdlog::warn("Client '{}' tried to ack unknown message '{}'", client_id, message_id);
    return {{"status", "error"}, {"message", "Unknown message_id"}};
  }

  spdlog::debug("Client '{}' acknowledged message '{}'", client_id, message_id);
  client.pending_acks.erase(msg_it);

  return {{"status", "ok"}, {"action", "ack"}};
}

void BabadzakiMQ::timeoutCheckLoop() {
  while (running) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::lock_guard<std::mutex> lock(clients_mutex);
    auto now = std::chrono::system_clock::now();

    for (auto& [client_id, client] : clients) {
      std::vector<std::string> expired_ids;

      for (auto& [msg_id, pending] : client.pending_acks) {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - pending.sent_at).count();

        if (elapsed >= pending.timeout_sec) {
          auto* queue = findQueue(pending.queue_name);
          if (queue) {
            queue->push(pending.message);
            spdlog::warn("Message '{}' timed out for client '{}', returned to queue '{}'", msg_id, client_id,
                         pending.queue_name);
          }
          expired_ids.push_back(msg_id);
        }
      }

      for (const auto& id : expired_ids) {
        client.pending_acks.erase(id);
      }
    }
  }
}

json BabadzakiMQ::handlePoll(const std::string& client_id) {
  std::lock_guard<std::mutex> lock(clients_mutex);
  auto client_it = clients.find(client_id);

  if (client_it == clients.end()) {
    return {{"status", "error"}, {"message", "Client not registered"}};
  }

  auto& client = client_it->second;

  for (auto& [queue_name, subscription] : client.subscriptions) {
    auto* queue = findQueue(queue_name);
    if (!queue)
      continue;

    if (subscription.qos == QoS::NoAck) {
      if (subscription.read_position < queue->size()) {
        Message msg;
        if (queue->peek(msg, subscription.read_position)) {
          subscription.read_position++;
          return {{"status", "ok"}, {"action", "poll"}, {"queue", queue_name}, {"message", msg.data}};
        }
      }
    } else {
      if (!queue->isEmpty()) {
        Message msg;
        if (queue->pop(msg)) {
          std::string msg_id = generateMessageId();

          PendingMessage pending;
          pending.message = msg;
          pending.queue_name = queue_name;
          pending.sent_at = std::chrono::system_clock::now();
          pending.timeout_sec = subscription.ack_timeout_sec;

          client.pending_acks[msg_id] = pending;

          return {{"status", "ok"},
                  {"action", "poll"},
                  {"queue", queue_name},
                  {"message", msg.data},
                  {"message_id", msg_id}};
        }
      }
    }
  }

  return {{"status", "ok"}, {"action", "poll"}, {"message", nullptr}};
}

void BabadzakiMQ::loop() {
  spdlog::info("Starting main event loop");

  while (running) {
    zmq::message_t identity;
    zmq::message_t request;

    try {
      auto result = socket.recv(identity, zmq::recv_flags::dontwait);
      if (!result) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        continue;
      }

      auto recv_result = socket.recv(request, zmq::recv_flags::none);
      if (!recv_result) {
        spdlog::warn("Failed to receive request body");
        continue;
      }

      std::string client_id(static_cast<char*>(identity.data()), identity.size());
      std::string request_str(static_cast<char*>(request.data()), request.size());
      spdlog::info("Received request from client '{}': {}", client_id, request_str);

      json j;
      try {
        j = json::parse(request_str);
      } catch (const json::parse_error& e) {
        spdlog::error("JSON parse error: {}", e.what(), request_str);

        json error_response = {{"status", "error"}, {"message", "Invalid JSON"}};

        socket.send(identity, zmq::send_flags::sndmore);
        socket.send(zmq::message_t(error_response.dump()), zmq::send_flags::none);
        continue;
      }

      std::string action = j.value("action", "");
      std::string queue_name = j.value("queue", "");
      int qos_level = j.value("qos", 0);
      int ack_timeout = j.value("ack_timeout", 30);
      QoS qos = (qos_level == 1) ? QoS::WithAck : QoS::NoAck;

      json response;

      if (action == "publish") {
        std::string message = j.value("message", "");
        response = handlePublish(client_id, queue_name, message, qos);
      } else if (action == "subscribe") {
        response = handleSubscribe(client_id, queue_name, qos, ack_timeout);
      } else if (action == "unsubscribe") {
        response = handleUnsubscribe(client_id, queue_name);
      } else if (action == "ack") {
        std::string message_id = j.value("message_id", "");
        response = handleAck(client_id, message_id);
      } else if (action == "poll") {
        response = handlePoll(client_id);
      } else {
        response = {{"status", "error"}, {"message", "Unknown action"}};
      }

      socket.send(identity, zmq::send_flags::sndmore);
      socket.send(zmq::message_t(response.dump()), zmq::send_flags::none);

    } catch (const zmq::error_t& e) {
      if (e.num() != EAGAIN) {
        spdlog::error("ZMQ error: {}", e.what());
      }
    } catch (const std::exception& e) {
      spdlog::error("Error in main loop: {}", e.what());
    }
  }

  spdlog::info("Main event loop stopped");
}

void BabadzakiMQ::cleanupLoop() {
  while (running) {
    for (auto& queue : queues) {
      queue->cleanupExpiredMessages();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}