#pragma once
#include <iomanip>
#include <libconvertor/Builder.hpp>
#include <sstream>

class JsonBuilder : public Builder {
 public:
  std::string build(const Node& node) override;
};
