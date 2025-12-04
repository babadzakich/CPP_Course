#pragma once
#include <string>
#include <map>

struct Order {
    int id;
    std::string city;
    std::map<int, int> productTypes; // type -> quantity
    int maxLoaders;
    
    Order(int orderId, const std::string& dest, int maxLoad) 
        : id(orderId), city(dest), maxLoaders(maxLoad) {}
};