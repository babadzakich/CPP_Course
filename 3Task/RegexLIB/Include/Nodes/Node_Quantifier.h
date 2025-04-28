//
// Created by babadzakich on 08.04.2025.
//

#ifndef NODE_QUANTIFIER_H
#define NODE_QUANTIFIER_H
#include <memory>

#include "Node_type.h"
#include "Regex_node.h"

class Node_Quantifier : public Regex_node {
 public:
  std::unique_ptr<Regex_node> child;
  char quantifier;
  Node_type type;
  Node_Quantifier(std::unique_ptr<Regex_node> ch, char quantifier)
      : child(std::move(ch)), quantifier(quantifier), type(char_to_node_type(quantifier)) {};
  bool match(const std::string& input, size_t& pos) const override;
  Node_type get_type() const override { return type; }
};

#endif  //NODE_QUANTIFIER_H
