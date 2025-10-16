#pragma once
#include <libconvertor/Parser.hpp>

class TomlParser : public Parser {
 public:
  Node parse(const std::string& input) override;

 private:
  size_t pos_ = 0;
  Node parseObject(std::string& input);
  Node parseArray(std::string& input);
  Node parseString(std::string& input);
  Node parseNumber(std::string& input);
  Node parseValue(std::string& input);
  std::string parseKey(std::string& input);
  void skipWhitespace(std::string& input);
};