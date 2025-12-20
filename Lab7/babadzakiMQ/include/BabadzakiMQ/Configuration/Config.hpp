#pragma once
#include <ctime>
#include <stdexcept>
#include <string>
#include <unordered_map>

enum class Durability { Memory, Disk };

enum class Ordering {
  None,
  FIFO,
};

struct Config {
  std::string name;
  Durability durability;
  Ordering ordering;
  time_t message_ttl;

  Config(const std::string& name_, std::string durability_ = "disk", std::string ordering_ = "fifo",
         time_t message_ttl_ = 0)
      : name(name_), message_ttl(message_ttl_ > 0 ? message_ttl_ : 0) {
    std::unordered_map<std::string, Durability> durability_map = {{"memory", Durability::Memory},
                                                                  {"disk", Durability::Disk}};
    std::unordered_map<std::string, Ordering> ordering_map = {{"none", Ordering::None}, {"fifo", Ordering::FIFO}};

    if (durability_map.find(durability_) == durability_map.end()) {
      throw std::invalid_argument("Invalid durability option: " + durability_);
    }
    durability = durability_map[durability_];

    if (ordering_map.find(ordering_) == ordering_map.end()) {
      throw std::invalid_argument("Invalid ordering option: " + ordering_);
    }
    ordering = ordering_map[ordering_];
  }
};
