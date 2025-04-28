//
// Created by babadzakich on 08.04.2025.
//

#include "Regex.h"
#include "Engine.h"

Regex::Regex(const std::string& pattern) {
  Engine parser(pattern);
  ast = parser.parse();
}

bool Regex::match(const std::string& input) const {
  if (size_t pos = 0; ast->match(input, pos)) {
    return pos == input.size();
  }
  return false;
}
