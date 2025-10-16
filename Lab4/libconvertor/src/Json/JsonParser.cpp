#include <libconvertor/Json/JsonParser.hpp>

Node JsonParser::parse(const std::string& input) {
  pos_ = 0;
  std::string inputCopy = input;
  Node result = parseValue(inputCopy);
  skipWhitespace(inputCopy);
  if (pos_ != inputCopy.size()) {
    throw std::runtime_error("Unexpected characters at end of input");
  }
  return result;
}

void JsonParser::skipWhitespace(std::string& input) {
  while (pos_ < input.size() && isspace(input[pos_])) {
    ++pos_;
  }
}

Node JsonParser::parseValue(std::string& input) {
  skipWhitespace(input);
  if (pos_ >= input.size()) {
    throw std::runtime_error("Unexpected end of input");
  }

  char c = input[pos_];

  switch (c) {
    case '{':
      return parseObject(input);
    case '[':
      return parseArray(input);
    case '"':
      return parseString(input);
    case 't':
      if (input.substr(pos_, 4) == "true") {
        pos_ += 4;
        return Node(true);
      }
      break;
    case 'f':
      if (input.substr(pos_, 5) == "false") {
        pos_ += 5;
        return Node(false);
      }
      break;
    default:
      if (isdigit(input[pos_]) || input[pos_] == '-') {
        return parseNumber(input);
      }
      break;
  }
  throw std::runtime_error("Invalid JSON value");
}

Node JsonParser::parseNumber(std::string& input) {
  bool isDouble = false;
  std::string number = "";

  if (input[pos_] == '-') {
    number += input[pos_++];
  }

  while (pos_ < input.size() && (isdigit(input[pos_]) || input[pos_] == '.')) {
    if (input[pos_] == '.') {
      if (isDouble)
        throw std::runtime_error("Multiple decimal points");
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
    throw std::runtime_error("Invalid number format");
  }
}

Node JsonParser::parseString(std::string& input) {
  ++pos_;
  std::string data;
  while (pos_ < input.size() && input[pos_] != '"') {
    if (input[pos_] == '\\') {
      ++pos_;
      if (pos_ >= input.size())
        throw std::runtime_error("Incomplete escape sequence");
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
        default:
          throw std::runtime_error("Unsupported escape sequence");
      }
      ++pos_;
    } else {
      data += input[pos_++];
    }
  }
  if (pos_ >= input.size()) {
    throw std::runtime_error("Unterminated string");
  }
  ++pos_;
  return Node(data);
}

Node JsonParser::parseArray(std::string& input) {
  ++pos_;
  Node::Array array;
  skipWhitespace(input);
  if (pos_ < input.size() && input[pos_] == ']') {
    ++pos_;
    return Node(array);
  }
  while (true) {
    array.push_back(parseValue(input));
    skipWhitespace(input);
    if (pos_ >= input.size()) {
      throw std::runtime_error("Unterminated array");
    }
    if (input[pos_] == ']') {
      ++pos_;
      break;
    }
    if (input[pos_] != ',') {
      throw std::runtime_error("Expected ',' or ']' in array");
    }
    ++pos_;
  }
  return Node(array);
}

Node JsonParser::parseObject(std::string& input) {
  ++pos_;
  Node::Object object;
  skipWhitespace(input);
  if (pos_ < input.size() && input[pos_] == '}') {
    ++pos_;
    return Node(object);
  }

  while (true) {
    skipWhitespace(input);
    if (pos_ >= input.size() || input[pos_] != '"') {
      throw std::runtime_error("Expected string key in object");
    }
    Node keyNode = parseString(input);
    if (!keyNode.value.holds_alternative<std::string>()) {
      throw std::runtime_error("Key is not a string");
    }
    std::string key = keyNode.value.get<std::string>();
    skipWhitespace(input);
    if (pos_ >= input.size() || input[pos_] != ':') {
      throw std::runtime_error("Expected ':' after key in object");
    }
    ++pos_;
    skipWhitespace(input);
    Node value = parseValue(input);
    object[key] = value;
    skipWhitespace(input);
    if (pos_ >= input.size()) {
      throw std::runtime_error("Unterminated object");
    }
    if (input[pos_] == '}') {
      ++pos_;
      break;
    }
    if (input[pos_] != ',') {
      throw std::runtime_error("Expected ',' or '}' in object");
    }
    ++pos_;
  }
  return Node(object);
}
