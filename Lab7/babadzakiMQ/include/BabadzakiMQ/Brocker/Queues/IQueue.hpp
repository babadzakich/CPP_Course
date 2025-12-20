#pragma once

#include <BabadzakiMQ/Brocker/Queues/Message.hpp>
#include <string>

class IQueue {
 public:
  virtual ~IQueue() = default;

  virtual void push(const Message& message) = 0;

  virtual bool pop(Message& message) = 0;

  virtual bool peek(Message& message, size_t index = 0) const = 0;

  virtual bool isEmpty() const = 0;

  virtual size_t size() const = 0;

  virtual void clear() = 0;

  virtual std::string getName() const = 0;

  virtual void cleanupExpiredMessages() = 0;
};