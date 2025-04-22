//
// Created by babadzakich on 08.04.2025.
//

#ifndef NODE_GROUP_H
#define NODE_GROUP_H
#include "Regex_node.h"
#include <unordered_set>


class Node_Group : public Regex_node {
    public:
    std::unordered_set<char> data;
    bool negated = false;
    Node_Group(bool negated = false) : negated(negated) {};

    bool match(const std::string &input, size_t &pos) const override;
    Node_type get_type() const override { return Node_type::GROUP; }
};



#endif //NODE_GROUP_H
