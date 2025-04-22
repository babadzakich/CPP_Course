//
// Created by babadzakich on 08.04.2025.
//

#ifndef NODE_REGULAR_H
#define NODE_REGULAR_H
#include "Regex_node.h"
#include <string>



class Node_Regular : public Regex_node {
    public:
    char letter;
    bool amatch;
    Node_Regular(char letter) : letter(letter), amatch(letter == '.') {}
    bool match(const std::string& input, size_t& pos) const override;
    Node_type get_type() const override { return Node_type::REGULAR; }
};



#endif //NODE_REGULAR_H
