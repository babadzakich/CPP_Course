#pragma once

#include <cassert>
#include <cctype>
#include <chrono>
#include <iostream>
#include <libconvertor/Variant/MyVariant.hpp>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

struct Node {
  using Object = std::map<std::string, Node>;
  using Array = std::vector<Node>;

  using Value = Variant<std::string, int64_t, double, bool, Object, Array, std::nullptr_t>;

  Value value;

  Node() : value(nullptr) {}
  Node(std::string s) : value(s) {}
  Node(int64_t i) : value(i) {}
  Node(double d) : value(d) {}
  Node(bool b) : value(b) {}
  Node(const Object& o) : value(o) {}
  Node(Object&& o) : value(std::move(o)) {}
  Node(Array a) : value(a) {}
};