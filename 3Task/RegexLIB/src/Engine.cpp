//
// Created by babadzakich on 08.04.2025.
//

#include "Engine.h"

#include <cstring>
#include <iostream>

#include "Nodes/Node_Group.h"
#include "Nodes/Node_Quantifier.h"
#include "Nodes/Node_Regular.h"
#include "Nodes/Node_Sequence.h"

std::unique_ptr<Regex_node> Engine::parse_sequence() {
  auto sequence = std::make_unique<Node_Sequence>();

  while (pos < pattern.size()) {
    auto element = parse_element();
    if (element) {
      sequence->children.push_back(std::move(element));
    }
  }

  return sequence;
}

std::unique_ptr<Regex_node> Engine::parse_element() {
  auto atom = parse_atom();
  if (!atom)
    return nullptr;

  if (pos < pattern.size() && strchr("*+?", pattern[pos])) {
    char quantifier = pattern[pos++];
    return std::make_unique<Node_Quantifier>(std::move(atom), quantifier);
  }

  return atom;
}

std::unique_ptr<Regex_node> Engine::parse_atom() {
  if (pos >= pattern.size())
    return nullptr;

  char c = pattern[pos];
  if (c == '[') {
    pos++;
    return parse_char_class();
  }

  if (c == '.' || (c != '*' && c != '+' && c != '?')) {
    pos++;
    return std::make_unique<Node_Regular>(c, false);
  }

  return nullptr;
}

std::unique_ptr<Regex_node> Engine::parse_char_class() {


  auto char_class = std::make_unique<Node_Group>();

  while (pos < pattern.size() && pattern[pos] != ']') {
    char c = pattern[pos++];
    if (c == '-') {
      const char prev = pattern[pos - 2];
      const char next = pattern[pos];
      for (char i = prev; i < next; i++) {
        char_class->data.insert(i);
      }
      continue;
    }
    char_class->data.insert(c);
  }

  if (pos >= pattern.size() || pattern[pos] != ']') {
    throw std::runtime_error("Unclosed character class");
  }
  pos++;

  return char_class;
}