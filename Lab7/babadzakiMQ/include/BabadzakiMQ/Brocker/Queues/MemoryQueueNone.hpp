#pragma once

#include <BabadzakiMQ/Brocker/Queues/IQueue.hpp>
#include <vector>
#include <mutex>
#include <chrono>

class MemoryQueueNone : public IQueue {
private:
    std::vector<Message> messages_;
    mutable std::mutex mutex_;
    std::string name_;
    std::chrono::seconds default_ttl_;

public:
    MemoryQueueNone(const std::string& name, 
                         std::chrono::seconds ttl = std::chrono::seconds(0));
    
    void push(const Message& message) override;
    bool pop(Message& message) override;
    bool peek(Message& message, size_t index = 0) const override;
    bool isEmpty() const override;
    size_t size() const override;
    void clear() override;
    std::string getName() const override;

    void cleanupExpiredMessages() override;
};
