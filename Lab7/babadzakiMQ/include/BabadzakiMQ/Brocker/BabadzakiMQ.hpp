#pragma once

#include <BabadzakiMQ/Brocker/Queues/MemoryQueueFIFO.hpp>
#include <BabadzakiMQ/Brocker/Queues/MemoryQueueNone.hpp>
#include <BabadzakiMQ/Brocker/Queues/DiskQueueFIFO.hpp>
#include <BabadzakiMQ/Brocker/Queues/DiskQueueNone.hpp>
#include <BabadzakiMQ/Configuration/Config.hpp>
#include <BabadzakiMQ/Brocker/Queues/IQueue.hpp>

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <zmq.hpp>

#include <atomic>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <random>
#include <sstream>
#include <iomanip>

using json = nlohmann::json;

enum class QoS {
    NoAck = 0,    
    WithAck = 1
};

struct QueueSubscription {
    std::string queue_name;
    QoS qos;
    int ack_timeout_sec;  // Таймаут ожидания подтверждения для QoS=1
    size_t read_position; // Позиция чтения для QoS=0
};

struct PendingMessage {
    Message message;
    std::string queue_name;
    std::chrono::system_clock::time_point sent_at;
    int timeout_sec;
};

struct ClientInfo {
    std::string identity;
    std::unordered_map<std::string, QueueSubscription> subscriptions; // queue_name -> subscription
    std::unordered_map<std::string, PendingMessage> pending_acks;    // message_id -> pending message
};

class BabadzakiMQ {
private:
    std::vector<std::unique_ptr<IQueue>> queues;
    
    std::thread cleanup_thread;
    std::atomic<bool> running{true};
    
    zmq::context_t context{8};
    zmq::socket_t socket;
    
    std::unordered_map<std::string, ClientInfo> clients;
    std::unordered_map<std::string, std::vector<std::string>> queue_subscribers;
    std::mutex clients_mutex;
    
    std::thread timeout_thread;
    
    void cleanupLoop();
    void timeoutCheckLoop();
    json handlePublish(const std::string& client_id, const std::string& queue_name, const std::string& message, QoS qos);
    json handleSubscribe(const std::string& client_id, const std::string& queue_name, QoS qos, int ack_timeout_sec);
    json handleUnsubscribe(const std::string& client_id, const std::string& queue_name);
    json handleAck(const std::string& client_id, const std::string& message_id);
    json handlePoll(const std::string& client_id);
    void deliverMessages();
    IQueue* findQueue(const std::string& name);
    std::string generateMessageId();

public:
    BabadzakiMQ(const std::vector<Config>& configs, const std::string& bind_address = "tcp://*:5555");
    ~BabadzakiMQ();
    void loop();
    void stop();
};

