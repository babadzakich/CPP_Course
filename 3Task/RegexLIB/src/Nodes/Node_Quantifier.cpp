//
// Created by babadzakich on 08.04.2025.
//

#include "Nodes/Node_Quantifier.h"

bool Node_Quantifier::match(const std::string &input, size_t &pos) const {
    size_t initial_pos = pos;

    switch (quantifier) {
        case '+':
            if (!child->match(input, pos)) return false;
            while (child->match(input, pos)) {
                if (pos == initial_pos) break;
                initial_pos = pos;
            }
            return true;
        case '*':
            while (child->match(input, pos)) {
                if (pos == initial_pos) break;
                initial_pos = pos;
            }
            return true;
        case '?':
            return child->match(input, pos);
        default:
            return false;
    }
}