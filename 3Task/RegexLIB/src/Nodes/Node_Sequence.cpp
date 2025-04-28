//
// Created by babadzakich on 08.04.2025.
//

#include <Nodes/Node_Sequence.h>
#include <iostream>
bool Node_Sequence::match(const std::string& input, size_t& pos) const {
  return my_match(input, pos, 0);
}

bool Node_Sequence::my_match(const std::string& input, size_t& pos, const int child) const {
  const size_t saved_pos = pos;
  for (int i = child; i < children.size(); i++) {
    const auto& child = children[i];
    if (child->get_type() == Node_type::QUESTION) {
      const size_t saved_pos2 = pos;

      if (my_match(input, pos, i + 1)) {
        return true;
      }
      pos = saved_pos2;
    }
    if (!child->match(input, pos)) {
      pos = saved_pos;
      return false;
    }
  }
  if (pos < input.size()) {
    return false;
  }
  return true;
}
