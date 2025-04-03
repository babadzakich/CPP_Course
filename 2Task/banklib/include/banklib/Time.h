#ifndef TIME_H
#define TIME_H

#include "banklib/Types.h"

class Time {
 private:
  int hours;
  int minutes;

 public:
  Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

  Time& operator++() {
    minutes++;
    hours += minutes / 60;
    minutes %= 60;
    hours %= 24;
    return *this;
  }

  Time operator++(int) {
    Time temp = *this;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }
    return temp;  // Return by value instead of reference
  }

  Time& operator+=(int minutes) {
    this->minutes += minutes;
    hours += this->minutes / 60;
    this->minutes %= 60;
    hours %= 24;
    return *this;
  }

  bool operator<(const Time& other) const {
    return hours < other.hours || (hours == other.hours && minutes < other.minutes);
  }

  bool operator>(const Time& other) const {
    return hours > other.hours || (hours == other.hours && minutes > other.minutes);
  }

  bool operator==(const Time& other) const { return hours == other.hours && minutes == other.minutes; }

  bool operator>=(const Time& other) const {
    return hours > other.hours || (hours == other.hours && minutes > other.minutes) ||
           (hours == other.hours && minutes == other.minutes);
  }

  bool operator<=(const Time& other) const {
    return hours < other.hours || (hours == other.hours && minutes < other.minutes) ||
           (hours == other.hours && minutes == other.minutes);
  }

  bool operator!=(const Time& other) const { return !(*this == other); }

  uint64_t getHours() const { return hours; }

  uint64_t getMinutes() const { return minutes; }
};

#endif