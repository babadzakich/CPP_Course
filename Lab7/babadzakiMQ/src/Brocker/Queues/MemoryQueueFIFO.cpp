#include <BabadzakiMQ/Brocker/Queues/MemoryQueueFIFO.hpp>

MemoryQueueFIFO::MemoryQueueFIFO(const std::string& name, 
                         std::chrono::seconds ttl)
    : name_(name)
    , default_ttl_(ttl) {}

void MemoryQueueFIFO::push(const Message& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push_back(message);
}

bool MemoryQueueFIFO::pop(Message& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (queue_.empty()) {
        return false;
    }
    
    message = queue_.front();
    queue_.pop_front();
    return true;
}

bool MemoryQueueFIFO::peek(Message& message, size_t index) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (index >= queue_.size()) {
        return false;
    }
    
    message = queue_[index];
    return true;
}

bool MemoryQueueFIFO::isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
}

size_t MemoryQueueFIFO::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.size();
}

void MemoryQueueFIFO::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    while (!queue_.empty()) {
        queue_.clear();
    }
}

std::string MemoryQueueFIFO::getName() const {
    return name_;
}

void MemoryQueueFIFO::cleanupExpiredMessages() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto now = std::chrono::system_clock::now();
    for (auto it = queue_.begin(); it != queue_.end(); ) {
        auto expiry_time = it->timestamp + default_ttl_;
        if (default_ttl_.count() > 0 && expiry_time < now) {
            it = queue_.erase(it);
        } else {
            ++it;
        }
    }
}