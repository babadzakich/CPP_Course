#pragma once

#include <vector>

#include <mutex>
#include <storageLib/DTO/Towar.hpp>

struct Address {
  int rack;
  int shelf;
  int slot;
};

class Pallet {
  std::vector<Towar> towars;
  Towar_type type;
  int total_quantity;
  std::mutex mtx;

 public:
  bool add_product(const Towar& towar);
  bool remove_product(int quantity);
  int get_total_quantity() const { return total_quantity; }
  Towar_type get_towar_type() const { return type; }
  std::vector<Towar> get_towar() const;
  bool is_empty() const { return total_quantity == 0; }
};
