#include <algorithm>
#include <iostream>
#include <libconvertor/Json/JsonBuilder.hpp>
#include <libconvertor/Json/JsonParser.hpp>
#include <libconvertor/Toml/TomlBuilder.hpp>
#include <libconvertor/Toml/TomlParser.hpp>
#include <libconvertor/Xml/XmlBuilder.hpp>
#include <libconvertor/Xml/XmlParser.hpp>
#include <map>
#include <cstdlib>

std::map<std::string, std::unique_ptr<Parser>> create_parsers() {
  std::map<std::string, std::unique_ptr<Parser>> m;
  m.emplace("json", std::make_unique<JsonParser>());
  m.emplace("toml", std::make_unique<TomlParser>());
  m.emplace("xml", std::make_unique<XmlParser>());
  return m;
}

std::map<std::string, std::unique_ptr<Builder>> create_builders() {
  std::map<std::string, std::unique_ptr<Builder>> m;
  m.emplace("json", std::make_unique<JsonBuilder>());
  m.emplace("toml", std::make_unique<TomlBuilder>());
  m.emplace("xml", std::make_unique<XmlBuilder>());
  return m;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " <input_format> <output_format>\n";
    std::cerr << "Supported formats: json, toml, xml\n";
    return EXIT_FAILURE;
  }

  std::string input_format = argv[1];
  std::string output_format = argv[2];
  std::transform(input_format.begin(), input_format.end(), input_format.begin(), ::tolower);
  std::transform(output_format.begin(), output_format.end(), output_format.begin(), ::tolower);
  std::string input;
  std::string line;
  while (std::getline(std::cin, line)) {
    input += line + "\n";
  }
  const std::map<std::string, std::unique_ptr<Parser>> parsers = create_parsers();
  const std::map<std::string, std::unique_ptr<Builder>> builders = create_builders();

  auto parser = parsers.at(input_format).get();
  auto builder = builders.at(output_format).get();
  if (!parser) {
    std::cerr << "Unsupported parser format. Supported formats: json, toml, xml\n";
    return EXIT_FAILURE;
  }
  if (!builder) {
    std::cerr << "Unsupported builder format. Supported formats: json, toml, xml\n";
    return EXIT_FAILURE;
  }
  try {
    std::string output = builder->build(parser->parse(input));
    std::cout << output << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}