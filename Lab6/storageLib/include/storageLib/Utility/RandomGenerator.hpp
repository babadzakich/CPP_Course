// RandomGenerator.h
#pragma once

#include <random>

class RandomGenerator {
 public:
  static std::mt19937 getGenerator();
  static int getRandom(int min, int max);
  static int getRandom(std::mt19937& gen, int min, int max);
  static std::uniform_int_distribution<> getDist(int min, int max);
};
