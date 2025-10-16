#pragma once

#include <libconvertor/Builder.hpp>

class XmlBuilder : public Builder {
 public:
  std::string build(const Node& node) override;
};