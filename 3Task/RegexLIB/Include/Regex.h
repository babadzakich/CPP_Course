//
// Created by babadzakich on 08.04.2025.
//

#ifndef REGEX_H
#define REGEX_H
#include <memory>
#include "Regex_node.h"




class Regex {
    std::unique_ptr<Regex_node> ast;
    public:
    Regex(const std::string& pattern);

    bool match(const std::string& input) const;

};



#endif //REGEX_H
