#include "Regex.h"
#include <iostream>

int main (int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <regex>" << std::endl;
        return 1;
    }
    Regex regex = Regex(argv[1]);
    std::string line;
    while (std::cin >> line) {
        if (regex.match(line)) {
            std::cout << line << " Matched" << std::endl;
        } else {
            std::cout << line << " Not Matched" << std::endl;
        }
    }
    return 0;
}