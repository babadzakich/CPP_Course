//
// Created by babadzakich on 08.04.2025.
//

#ifndef ENGINE_H
#define ENGINE_H
#include <memory>
#include <Regex_node.h>

#include <string>
#include <vector>


class Engine {
    std::string pattern;
    size_t pos;

    std::unique_ptr<Regex_node> parse_sequence();
    std::unique_ptr<Regex_node> parse_element();
    std::unique_ptr<Regex_node> parse_atom();
    std::unique_ptr<Regex_node> parse_char_class();

public:
    Engine(const std::string& pat) : pattern(pat), pos(0) {}

    std::unique_ptr<Regex_node> parse() {
        auto node = parse_sequence();
        if (pos < pattern.size()) {
            throw std::runtime_error("Unexpected character in pattern");
        }
        return node;
    }
};



#endif //ENGINE_H
