//
// Created by babadzakich on 08.04.2025.
//

#include "Nodes/Node_Group.h"

bool Node_Group::match(const std::string& input, size_t& pos) const {
  if (pos >= input.size()) {
    return false;
  }

  if (const bool contains = data.contains(input[pos]); contains) {
    pos++;
    return true;
  }
  return false;
}
