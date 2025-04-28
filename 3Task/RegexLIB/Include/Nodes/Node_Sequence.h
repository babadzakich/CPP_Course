//
// Created by babadzakich on 08.04.2025.
//

#ifndef NODE_SEQUENCE_H
#define NODE_SEQUENCE_H
#include <memory>
#include "Regex_node.h"

class Node_Sequence : public Regex_node {
 public:
  std::vector<std::unique_ptr<Regex_node>> children;
  bool match(const std::string& input, size_t& pos) const override;
  Node_type get_type() const override { return Node_type::SEQUENCE; }
  bool my_match(const std::string& input, size_t& pos, int child) const;
};

#endif  //NODE_SEQUENCE_H
