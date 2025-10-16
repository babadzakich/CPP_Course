#pragma once
#include <libconvertor/Node.hpp>
#include <string>

class Builder {
 public:
  virtual ~Builder() = default;
  virtual std::string build(const Node& node) = 0;
};