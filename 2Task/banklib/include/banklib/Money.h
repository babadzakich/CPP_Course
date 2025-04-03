#ifndef MONEY_H
#define MONEY_H

#include "banklib/Types.h"

class Money {
 private:
  double value;

  double round(double value) { return (((long long int)(value * 10000) + 5 * (value < 0.0 ? -1 : 1)) / 10) / 1000.0; }

 public:
  Money() : value(0) {};
  Money(uint64_t value1, uint64_t value2) { value = round(value1 + value2 / 100.0); }

  Money& operator+=(const Money& other) {
    value += other.value;
    value = round(value);
    return *this;
  }

  Money& operator-=(const Money& other) {
    value -= other.value;
    value = round(value);
    return *this;
  }

  Money& operator*=(const Money& other) {
    value *= other.value;
    value = round(value);
    return *this;
  }

  Money& operator/=(const Money& other) {
    value /= other.value;
    value = round(value);
    return *this;
  }

  bool operator<(const Money& other) const { return value < other.value; }

  bool operator<(const double& other) const { return value < other; }

  bool operator>(const Money& other) const { return value > other.value; }

  bool operator>(const double& other) const { return value > other; }

  bool operator==(const Money& other) const { return value == other.value; }

  bool operator==(const double& other) const { return value == other; }

  bool operator!=(const Money& other) const { return !(*this == other); }

  bool operator!=(const double& other) const { return !(*this == other); }

  bool operator<=(const Money& other) const { return *this < other || *this == other; }

  bool operator>=(const Money& other) const { return *this > other || *this == other; }

  Money operator+(const Money& other) const {
    Money temp = *this;
    temp += other;
    return temp;
  }

  Money operator+(const double& other) const {
    Money temp = *this;
    temp += Money(other, 0);
    return temp;
  }

  Money operator-(const Money& other) const {
    Money temp = *this;
    temp -= other;
    return temp;
  }

  Money operator-(const double& other) const {
    Money temp = *this;
    temp -= Money(other, 0);
    return temp;
  }

  Money operator*(const Money& other) const {
    Money temp = *this;
    temp *= other;
    return temp;
  }

  Money operator*(const double& other) const {
    Money temp = *this;
    temp *= Money(other, 0);
    return temp;
  }

  Money operator/(const Money& other) const {
    Money temp = *this;
    temp /= other;
    return temp;
  }

  Money operator/(const double& other) const {
    Money temp = *this;
    temp /= Money(other, 0);
    return temp;
  }

  double getValue() const { return value; }

  uint64_t getVal1() const { return (uint64_t)value; }

  uint64_t getVal2() const { return (uint64_t)(value * 1000) % 1000; }
};

#endif  // MONEY_H