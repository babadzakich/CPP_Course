#include <BabadzakiMQ/Brocker/Queues/MemoryQueueNone.hpp>

MemoryQueueNone::MemoryQueueNone(const std::string& name, 
                                           std::chrono::seconds ttl)
    : name_(name)
    , default_ttl_(ttl) {}

void MemoryQueueNone::push(const Message& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    messages_.push_back(message);
}

bool MemoryQueueNone::pop(Message& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (messages_.empty()) {
        return false;
    }
    
    message = messages_.back();
    messages_.pop_back();
    return true;
}

bool MemoryQueueNone::peek(Message& message, size_t index) const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (index >= messages_.size()) {
        return false;
    }
    
    message = messages_[index];
    return true;
}

bool MemoryQueueNone::isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return messages_.empty();
}

size_t MemoryQueueNone::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return messages_.size();
}

void MemoryQueueNone::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    messages_.clear();
}

std::string MemoryQueueNone::getName() const {
    return name_;
}

void MemoryQueueNone::cleanupExpiredMessages() {
    std::lock_guard<std::mutex> lock(mutex_);
    
    auto now = std::chrono::system_clock::now();
    size_t write_idx = 0;
    for (size_t read_idx = 0; read_idx < messages_.size(); ++read_idx) {
        auto expiry_time = messages_[read_idx].timestamp + default_ttl_;
        if (expiry_time >= now) {
            if (write_idx != read_idx) {
                messages_[write_idx] = std::move(messages_[read_idx]);
            }
            ++write_idx;
        }
    }
    messages_.resize(write_idx);
}