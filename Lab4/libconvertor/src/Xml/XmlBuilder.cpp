#include <libconvertor/Xml/XmlBuilder.hpp>

std::string XmlBuilder::build(const Node& node) {
  std::ostringstream oss;

  if (node.value.holds_alternative<int64_t>()) {
    oss << node.value.get<int64_t>();
  } else if (node.value.holds_alternative<double>()) {
    oss << node.value.get<double>();
  } else if (node.value.holds_alternative<bool>()) {
    oss << (node.value.get<bool>() ? "true" : "false");
  } else if (node.value.holds_alternative<Node::Object>()) {

    for (auto& [key, value] : node.value.get<Node::Object>()) {
      if (value.value.holds_alternative<Node::Array>()) {
        oss << "<" << key << "-array>\n";
        for (auto& item : value.value.get<Node::Array>()) {
          oss << "<" << key << ">" << build(item) << "</" << key << ">\n";
        }
        oss << "</" << key << ">\n";
        continue;
      }
      oss << "\n<" << key << ">" << build(value) << "</" << key << ">\n";
    }
  } else if (node.value.holds_alternative<Node::Array>()) {
    for (auto& value : node.value.get<Node::Array>()) {
      oss << build(value) << '\n';
    }
  } else if (node.value.holds_alternative<std::string>()) {
    oss << node.value.get<std::string>();
  } else {
    throw std::runtime_error("unsupported operation");
  }

  return oss.str();
}