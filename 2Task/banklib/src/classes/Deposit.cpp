//
// Created by babadzakich on 14.03.2025.
//

#include "banklib/classes/Deposit.h"

Number Deposit::getId() const {
  return id;
}

double Deposit::getDailyProcent() const {
  return procent / 365;
}

double Deposit::getMonthlyProcent() const {
  return procent / 12;
}

double Deposit::getQuarterlyProcent() const {
  return procent / 4;
}

double Deposit::getSemiannuallyProcent() const {
  return procent / 2;
}

double Deposit::getAnnuallyProcent() const {
  return procent;
}

Deposit_Type Deposit::getType() const {
  return type;
}

Date Deposit::getStartDate() const {
  return startDate;
}

Number Deposit::getDuration() const {
  return duration;
}

Deposit::Deposit(Number id, double procent, Deposit_Type type, Date startDate, Number duration)
    : id(id), procent(procent), type(type), startDate(startDate), duration(duration) {}
