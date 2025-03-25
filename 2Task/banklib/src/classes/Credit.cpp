//
// Created by babadzakich on 14.03.2025.
//

#include "banklib/classes/Credit.h"

Credit::Credit(uint64_t id, double procent, Credit_Type type, Money received, uint64_t accountId)
    : id(id), procent(procent), type(type), received(received), accountId(accountId) {}

uint64_t Credit::getId() const {
  return id;
}

double Credit::getProcent() const {
  return procent;
}

Credit_Type Credit::getType() const {
  return type;
}

uint64_t Credit::getAccountId() const {
  return accountId;
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
