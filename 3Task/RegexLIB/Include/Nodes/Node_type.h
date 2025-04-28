//
// Created by babadzakich on 22.04.2025.
//

#ifndef NODE_TYPE_H
#define NODE_TYPE_H

enum class Node_type { REGULAR, GROUP, QUESTION, STAR, PLUS, SEQUENCE };
static Node_type char_to_node_type(char c) {
  switch (c) {
    case '*':
      return Node_type::STAR;
    case '?':
      return Node_type::QUESTION;
    case '+':
      return Node_type::STAR;
    default:
      return Node_type::REGULAR;
  }
}
#endif  //NODE_TYPE_H
