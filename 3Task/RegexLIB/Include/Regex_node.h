//
// Created by babadzakich on 07.04.2025.
//

#ifndef REGEX_NODE_H
#define REGEX_NODE_H

#include <unordered_map>
#include <vector>
#include <string>

#include "Nodes/Node_type.h"

class Regex_node {
public:
    virtual ~Regex_node() = default;
    virtual bool match(const std::string& input, size_t& pos) const = 0;
    virtual Node_type get_type() const = 0;
};


#endif //REGEX_NODE_H
