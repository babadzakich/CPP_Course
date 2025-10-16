#pragma once

#include <libconvertor/Node.hpp>

class Parser {
 public:
  virtual ~Parser() = default;
  virtual Node parse(const std::string& input) = 0;
};
