#include <libconvertor/Toml/TomlBuilder.hpp>

std::string TomlBuilder::build(const Node& node) {
  std::ostringstream oss;

  if (node.value.holds_alternative<std::string>()) {
    oss << '"' << node.value.get<std::string>() << '"';
  } else if (node.value.holds_alternative<int64_t>()) {
    oss << node.value.get<int64_t>();
  } else if (node.value.holds_alternative<double>()) {
    oss << node.value.get<double>();
  } else if (node.value.holds_alternative<bool>()) {
    oss << (node.value.get<bool>() ? "true" : "false");
  } else if (node.value.holds_alternative<Node::Object>()) {
    for (auto& [key, value] : node.value.get<Node::Object>()) {
      if (value.value.holds_alternative<Node::Object>()) {
        oss << "[" << key << "]\n" << build(value);
      } else if (value.value.holds_alternative<Node::Array>() && !value.value.get<Node::Array>().empty() &&
                 value.value.get<Node::Array>()[0].value.holds_alternative<Node::Object>()) {
        Node::Array arr = value.value.get<Node::Array>();
        for (const auto& item : arr) {
          oss << "[[" << key << "]]\n" << build(item);
        }
      } else {
        oss << key << " = " << build(value) << "\n";
      }
    }
  } else if (node.value.holds_alternative<Node::Array>()) {
    Node::Array arr = node.value.get<Node::Array>();
    oss << '[';
    for (int i = 0; i < arr.size(); i++) {
      oss << ' ' << build(arr[i]);
      if (i != arr.size() - 1)
        oss << ',';
    }
    oss << ']';
  } else {
    throw std::runtime_error("Unsupported node type");
  }

  return oss.str();
}