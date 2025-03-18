//
// Created by babadzakich on 14.03.2025.
//

#include "banklib/classes/Credit.h"

Credit::Credit(Number id, double procent, Credit_Type type, Money received)
    : id(id), procent(procent), type(type), received(received) {}

Number Credit::getId() const {
  return id;
}

double Credit::getProcent() const {
  return procent;
}

Credit_Type Credit::getType() const {
  return type;
}

double Credit::getDailyProcent() const {
  return procent / 365.0;
}

double Credit::getMonthlyProcent() const {
  return procent / 12.0;
}

double Credit::getQuarterlyProcent() const {
  return procent / 4.0;
}

double Credit::getSemiannualProcent() const {
  return procent / 2.0;
}

double Credit::getAnnualProcent() const {
  return procent;
}
