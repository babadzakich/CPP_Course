#include <libconvertor/Toml/TomlParser.hpp>

Node TomlParser::parse(const std::string& input) {
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

void TomlParser::skipWhitespace(std::string& input) {
  while (pos_ < input.size()) {
    if (isspace(input[pos_])) {
      if (input[pos_] == '\r' && pos_ + 1 < input.size() && input[pos_ + 1] == '\n') {
        pos_ += 2;
      } else {
        ++pos_;
      }
    } else if (input[pos_] == '#') {
      while (pos_ < input.size() && input[pos_] != '\n' &&
             !(input[pos_] == '\r' && pos_ + 1 < input.size() && input[pos_ + 1] == '\n')) {
        ++pos_;
      }
    } else {
      break;
    }
  }
}

std::string TomlParser::parseKey(std::string& input) {
  skipWhitespace(input);
  if (pos_ >= input.size()) {
    throw std::runtime_error("Expected key at position " + std::to_string(pos_));
  }
  if (input[pos_] == '"') {
    Node keyNode = parseString(input);
    if (!keyNode.value.holds_alternative<std::string>()) {
      throw std::runtime_error("Key is not a string");
    }
    return keyNode.value.get<std::string>();
  }
  std::string key;
  while (pos_ < input.size()) {
    char c = input[pos_];
    if (isalnum(c) || c == '_' || c == '-') {
      key += c;
      ++pos_;
    } else {
      break;
    }
  }
  if (key.empty()) {
    throw std::runtime_error("Invalid key at position " + std::to_string(pos_));
  }
  return key;
}

Node TomlParser::parseObject(std::string& input) {
  Node::Object object;
  while (pos_ < input.size()) {
    skipWhitespace(input);
    if (pos_ >= input.size()) {
      break;
    }

    if (input[pos_] == '[') {
      ++pos_;
      bool isArrayOfTables = false;
      if (pos_ < input.size() && input[pos_] == '[') {
        isArrayOfTables = true;
        ++pos_;
      }
      std::string path = parseKey(input);

      if (isArrayOfTables) {
        if (pos_ >= input.size() || input[pos_] != ']') {
          throw std::runtime_error("Expected closing ']]' for array of tables");
        }
        ++pos_;
      }

      if (pos_ >= input.size() || input[pos_] != ']') {
        throw std::runtime_error("Expected closing ']' for table");
      }

      ++pos_;
      skipWhitespace(input);
      Node::Object table;
      while (pos_ < input.size()) {
        skipWhitespace(input);
        if (pos_ < input.size() && input[pos_] == '[') {
          // Началась новая таблица — выходим
          break;
        }
        if (pos_ >= input.size())
          break;

        auto key = parseKey(input);
        skipWhitespace(input);
        if (pos_ >= input.size() || input[pos_] != '=') {
          throw std::runtime_error("Expected '=' after key in table '" + path + "'");
        }
        ++pos_;
        skipWhitespace(input);
        Node value = parseValue(input);
        table[key] = value;
      }

      if (isArrayOfTables) {
        if (object.find(path) == object.end()) {
          object[path] = Node(Node::Array{});
        }
        if (!object[path].value.holds_alternative<Node::Array>()) {
          throw std::runtime_error("Expected array for key '" + path + "'");
        }
        object[path].value.get<Node::Array>().push_back(Node(table));
      } else {
        object[path] = Node(table);
      }

      continue;
    } else {
      auto path = parseKey(input);
      skipWhitespace(input);
      if (pos_ >= input.size() || input[pos_] != '=') {
        throw std::runtime_error("Expected '=' after key at position " + std::to_string(pos_));
      }
      ++pos_;
      skipWhitespace(input);
      Node value = parseValue(input);
      object[path] = value;
    }
    skipWhitespace(input);
  }
  return Node(object);
}

Node TomlParser::parseNumber(std::string& input) {
  std::string number;
  bool isDouble = false;
  bool isBin = false;

  if (input[pos_] == '0' && pos_ + 1 < input.size()) {
    if (input[pos_ + 1] == 'b') {
      isBin = true;
      number += input[pos_++];
      number += input[pos_++];
    }
  }

  if (input[pos_] == '-' || input[pos_] == '+') {
    number += input[pos_++];
  }
  while (pos_ < input.size()) {
    char c = input[pos_];
    if (std::isdigit(c)) {
      number += c;
      ++pos_;
    } else if (c == '.' && !isDouble && !isBin) {
      isDouble = true;
      number += c;
      ++pos_;
    } else if ((c == 'e' || c == 'E') && !isBin) {
      isDouble = true;
      number += c;
      ++pos_;
      if (pos_ < input.size() && (input[pos_] == '-' || input[pos_] == '+')) {
        number += input[pos_++];
      }
    } else {
      break;
    }
  }

  try {
    if (isDouble) {
      return Node(std::stod(number));
    } else if (isBin) {
      return Node(std::stoll(number, nullptr, 2));
    } else {
      return Node(std::stoll(number));
    }
  } catch (...) {
    throw std::runtime_error("Invalid number format at position " + std::to_string(pos_));
  }
}

Node TomlParser::parseString(std::string& input) {
  if (pos_ >= input.size()) {
    throw std::runtime_error("Expected string at position " + std::to_string(pos_));
  }
  ++pos_;
  std::string data;

  while (pos_ < input.size()) {
    if (input[pos_] == '"') {
      ++pos_;
      return Node(data);
    }

    if (input[pos_] == '\\') {
      ++pos_;
      if (pos_ >= input.size()) {
        throw std::runtime_error("Incomplete escape sequence at position " + std::to_string(pos_));
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
  ++pos_;
  return Node(data);
}

Node TomlParser::parseValue(std::string& input) {
  skipWhitespace(input);
  if (pos_ >= input.size()) {
    throw std::runtime_error("Unexpected end of input while parsing value at position " + std::to_string(pos_));
  }
  char c = input[pos_];
  switch (c) {
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
    case '[':
      return parseArray(input);
    default:
      if (c == '-' || c == '+' || isdigit(c)) {
        return parseNumber(input);
      }
      break;
  }
  throw std::runtime_error("Unexpected character while parsing value at position " + std::to_string(pos_) + " '" + c +
                           "'");
}

Node TomlParser::parseArray(std::string& input) {
  ++pos_;
  skipWhitespace(input);
  Node::Array array;
  if (pos_ < input.size() && input[pos_] == ']') {
    ++pos_;
    return Node(array);
  }
  while (true) {
    array.push_back(parseValue(input));
    skipWhitespace(input);
    if (pos_ >= input.size()) {
      throw std::runtime_error("Unterminated array at position " + std::to_string(pos_));
    }
    if (input[pos_] == ']') {
      ++pos_;
      break;
    }
    if (input[pos_] != ',') {
      throw std::runtime_error("Expected ',' or ']' in array at position " + std::to_string(pos_));
    }
    ++pos_;
    skipWhitespace(input);
  }
  return Node(array);
}