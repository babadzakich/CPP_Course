#pragma once

enum class Towar_type { Light, Medium, Heavy };

struct Towar {
  Towar_type type;
  int quantity;

  int getMaxPerPallet() {
    switch (type) {
      case Towar_type::Light:
        return 100;
      case Towar_type::Medium:
        return 30;
      case Towar_type::Heavy:
        return 4;
    }
    return 0;
  }
};
