//
// Created by babadzakich on 08.04.2025.
//

#ifndef NODE_GROUP_H
#define NODE_GROUP_H
#include <unordered_set>
#include "Regex_node.h"

class Node_Group : public Regex_node {
 public:
  std::unordered_set<char> data;
  Node_Group() {};

  bool match(const std::string& input, size_t& pos) const override;
  Node_type get_type() const override { return Node_type::GROUP; }
};

#endif  //NODE_GROUP_H
