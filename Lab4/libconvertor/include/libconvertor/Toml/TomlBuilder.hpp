#pragma once
#include <libconvertor/Builder.hpp>

class TomlBuilder : public Builder {
 public:
  std::string build(const Node& node) override;
};