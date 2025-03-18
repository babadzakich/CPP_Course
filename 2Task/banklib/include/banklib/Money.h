#ifndef MONEY_H
#define MONEY_H

#include "banklib/Types.h"

class Money {
 private:
  double value;
  int val1;
  int val2;

  double round(double value) { return (((long long int)(value * 10000) + 5 * (value < 0.0 ? -1 : 1)) / 10) / 1000.0; }

 public:
  Money(Number value1, Number value2) {
    value = round(value1 + value2 / 100.0);
    val1 = value1;
    val2 = value2;
  }

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

  Number getVal1() const { return val1; }

  Number getVal2() const { return val2; }
};

#endif  // MONEY_H