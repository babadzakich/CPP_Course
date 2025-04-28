//
// Created by babadzakich on 08.04.2025.
//

#include "Nodes/Node_Regular.h"

#include <regex>

bool Node_Regular::match(const std::string& input, size_t& pos) const {
  if (pos >= input.size()) {
    return false;
  }
  if (input[pos] == letter || amatch) {
    pos++;
    return true;
  }
  return false;
}
