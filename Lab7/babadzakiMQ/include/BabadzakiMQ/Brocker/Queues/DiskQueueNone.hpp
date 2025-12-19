#pragma once

#include <BabadzakiMQ/Brocker/Queues/IQueue.hpp>
#include <filesystem>
#include <mutex>

class DiskQueueNone : public IQueue {
private:
    std::filesystem::path storage_dir_;
    std::string name_;
    std::chrono::seconds default_ttl_;
    mutable std::mutex mutex_;
    
    size_t next_id_;
    
    std::string generateFilename() const;
    std::chrono::system_clock::time_point parseTimestampFromFilename(const std::string& filename) const;
    
public:
    DiskQueueNone(const std::string& name, 
              std::chrono::seconds ttl = std::chrono::seconds(0),
              const std::filesystem::path& storage_path = "./queues");
    
    void push(const Message& message) override;
    bool pop(Message& message) override;
    bool peek(Message& message, size_t index = 0) const override;
    bool isEmpty() const override;
    size_t size() const override;
    void clear() override;
    std::string getName() const override;
    void cleanupExpiredMessages() override;
};
