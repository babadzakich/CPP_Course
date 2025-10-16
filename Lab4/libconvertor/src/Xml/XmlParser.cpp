#include <libconvertor/Xml/XmlParser.hpp>

Node XmlParser::parse(const std::string& input) {
  std::string mutable_input = input;
  pos_ = 0;
  skipWhitespace(mutable_input);
  Node result = parseObject(mutable_input);
  skipWhitespace(mutable_input);
  if (pos_ != mutable_input.size()) {
    throw std::runtime_error("Unexpected characters at end of input");
  }
  return result;
}

std::string XmlParser::parseTagName(std::string& input) {
  skipWhitespace(input);
  if (pos_ >= input.size() || !(isalnum(input[pos_]) || input[pos_] == '-' || input[pos_] == '_')) {
    throw std::runtime_error("Expected tag name at position " + std::to_string(pos_));
  }
  std::string tagName;
  while (pos_ < input.size() && (isalnum(input[pos_]) || input[pos_] == '-' || input[pos_] == '_')) {
    tagName += input[pos_++];
  }
  if (tagName.empty()) {
    throw std::runtime_error("Invalid tag name at position " + std::to_string(pos_));
  }
  skipWhitespace(input);
  return tagName;
}

Node XmlParser::parseObject(std::string& input) {
  Node::Object obj;
  while (pos_ < input.size()) {
    skipWhitespace(input);
    if (pos_ >= input.size()) {
      break;
    }
    if (input[pos_] == '<') {
      if (pos_ + 1 < input.size() && input[pos_ + 1] == '/') {
        break;
      }
      ++pos_;
      std::string tagName = parseTagName(input);
      skipWhitespace(input);
      if (pos_ >= input.size() || input[pos_] != '>') {
        throw std::runtime_error("Expected '>' after tag name '" + tagName + "' at position " + std::to_string(pos_));
      }
      ++pos_;
      skipWhitespace(input);

      if (tagName.empty()) {
        throw std::runtime_error("Empty tag name at position " + std::to_string(pos_));
      }

      if (tagName.size() > 6 && tagName.substr(tagName.size() - 6) == "-array") {
        tagName = tagName.substr(0, tagName.size() - 6);
        Node arr = parseArray(input);

        obj[tagName] = Node(arr);

        pos_ += 2;

        std::string endTagName = parseTagName(input);
        if (endTagName != tagName) {
          throw std::runtime_error("Mismatched end tag: expected </" + tagName + "> but found </" + endTagName +
                                   "> at position " + std::to_string(pos_));
        }
        skipWhitespace(input);
        if (pos_ >= input.size() || input[pos_] != '>') {
          throw std::runtime_error("Expected '>' after end tag name '" + endTagName + "' at position " +
                                   std::to_string(pos_));
        }
        ++pos_;
        continue;
      }
      Node value = parseValue(input);
      obj[tagName] = value;

      if (pos_ < input.size() && input[pos_] == '<' && pos_ + 1 < input.size() && input[pos_ + 1] == '/') {
        pos_ += 2;
        std::string endTagName = parseTagName(input);
        if (endTagName != tagName) {
          throw std::runtime_error("Mismatched end tag: expected </" + tagName + "> but found </" + endTagName +
                                   "> at position " + std::to_string(pos_));
        }
        skipWhitespace(input);
        if (pos_ >= input.size() || input[pos_] != '>') {
          throw std::runtime_error("Expected '>' after end tag name '" + endTagName + "' at position " +
                                   std::to_string(pos_));
        }
        ++pos_;
      } else {
        throw std::runtime_error("Expected closing tag for '" + tagName + "' at position " + std::to_string(pos_));
      }
    } else {
      break;
    }
  }
  return Node(obj);
}

void XmlParser::skipWhitespace(std::string& input) {
  while (pos_ < input.size() && isspace(input[pos_])) {
    ++pos_;
  }
}

Node XmlParser::parseValue(std::string& input) {
  skipWhitespace(input);
  if (pos_ >= input.size()) {
    throw std::runtime_error("Unexpected end of input while parsing value at position " + std::to_string(pos_));
  }
  if (input[pos_] == '<') {
    return parseObject(input);
  } else if (input[pos_] == '"') {
    ++pos_;
    Node str = parseString(input);
    if (pos_ >= input.size() || input[pos_] != '"') {
      throw std::runtime_error("Expected closing '\"' for string at position " + std::to_string(pos_));
    }
    ++pos_;
    return str;
  } else if (isdigit(input[pos_]) || input[pos_] == '-' || input[pos_] == '+') {
    return parseNumber(input);
  } else if (input.substr(pos_, 4) == "true") {
    pos_ += 4;
    return Node(true);
  } else if (input.substr(pos_, 5) == "false") {
    pos_ += 5;
    return Node(false);
  } else if (isalpha(input[pos_])) {
    return parseString(input);
  } else {
    throw std::runtime_error("Invalid XML value at position " + std::to_string(pos_));
  }
}

Node XmlParser::parseString(std::string& input) {
  std::string data;
  while (pos_ < input.size() && input[pos_] != '<') {
    if (input[pos_] == '\\') {
      ++pos_;
      if (pos_ >= input.size()) {
        throw std::runtime_error("Unterminated escape sequence at position " + std::to_string(pos_));
      }
      switch (input[pos_]) {
        case '"':
          data += '"';
          break;
        case '\\':
          data += '\\';
          break;
        case 'n':
          data += '\n';
          break;
        case 't':
          data += '\t';
          break;
        case 'r':
          data += '\r';
          break;
        case 'b':
          data += '\b';
          break;
        case 'f':
          data += '\f';
          break;
        default:
          throw std::runtime_error("Unsupported escape sequence at position " + std::to_string(pos_));
      }
      ++pos_;
    } else {
      data += input[pos_++];
    }
  }
  if (pos_ >= input.size()) {
    throw std::runtime_error("Unterminated string at position " + std::to_string(pos_));
  }
  return Node(data);
}

Node XmlParser::parseNumber(std::string& input) {
  bool isDouble = false;
  std::string number;
  if (input[pos_] == '-' || input[pos_] == '+') {
    number += input[pos_++];
  }
  while (pos_ < input.size() && (isdigit(input[pos_]) || input[pos_] == '.')) {
    if (input[pos_] == '.') {
      if (isDouble) {
        throw std::runtime_error("Multiple decimal points in number at position " + std::to_string(pos_));
      }
      isDouble = true;
    }
    number += input[pos_++];
  }
  try {
    if (isDouble) {
      return Node(std::stod(number));
    } else {
      return Node(std::stoll(number));
    }
  } catch (...) {
    throw std::runtime_error("Invalid number format at position " + std::to_string(pos_));
  }
}

Node XmlParser::parseArray(std::string& input) {
  Node::Array arr;
  while (true) {
    skipWhitespace(input);
    if (pos_ < input.size() && input[pos_] == '<' && pos_ + 1 < input.size() && input[pos_ + 1] == '/') {
      break;
    }
    if (pos_ >= input.size() || input[pos_] != '<') {
      throw std::runtime_error("Expected '<>' in array");
    }
    ++pos_;
    std::string tagName = parseTagName(input);

    if (pos_ >= input.size() || input[pos_] != '>') {
      throw std::runtime_error("Expected '>' after tag name in array");
    }
    ++pos_;
    Node value = parseValue(input);
    arr.push_back(value);
    if (pos_ >= input.size() || input[pos_] != '<' || input[pos_ + 1] != '/') {
      throw std::runtime_error("Expected closing tag </ > in array");
    }
    pos_ += 2;
    std::string endTag = parseTagName(input);
    ++pos_;
  }
  return Node(arr);
}