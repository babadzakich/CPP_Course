#pragma once
#include <libconvertor/Parser.hpp>

class JsonParser : public Parser {
 public:
  Node parse(const std::string& input) override;

 private:
  Node parseObject(std::string& input);
  Node parseArray(std::string& input);
  Node parseString(std::string& input);
  Node parseNumber(std::string& input);
  Node parseValue(std::string& input);
  void skipWhitespace(std::string& input);

  size_t pos_ = 0;
};
