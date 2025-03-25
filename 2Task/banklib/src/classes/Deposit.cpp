//
// Created by babadzakich on 14.03.2025.
//

#include "banklib/classes/Deposit.h"

uint64_t Deposit::getId() const {
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

uint64_t Deposit::getDuration() const {
  return duration;
}

uint64_t Deposit::getAccountId() const {
  return accountId;
}

Deposit::Deposit(uint64_t id, double procent, Deposit_Type type, Date startDate, uint64_t duration, uint64_t accountId)
    : id(id), procent(procent), type(type), startDate(startDate), duration(duration), accountId(accountId) {}
