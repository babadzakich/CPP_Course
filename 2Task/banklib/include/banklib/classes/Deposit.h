//
// Created by babadzakich on 14.03.2025.
//

#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Deposit_Type.h"

class Deposit {
 private:
  uint64_t id;
  double procent;
  Deposit_Type type;
  Date startDate;
  uint64_t duration;
  uint64_t accountId;

 public:
  uint64_t getId() const;
  double getDailyProcent() const;
  double getMonthlyProcent() const;
  double getQuarterlyProcent() const;
  double getSemiannuallyProcent() const;
  double getAnnuallyProcent() const;
  Deposit_Type getType() const;
  Date getStartDate() const;
  uint64_t getDuration() const;
  uint64_t getAccountId() const;
  Deposit(uint64_t id, double procent, Deposit_Type type, Date startDate, uint64_t duration, uint64_t accountId);
  Deposit() : id(0), procent(0), type(Deposit_Type::COMPOUNDED_ANUALLY_MIN), startDate(0), duration(0), accountId(0) {};
};

#endif  //DEPOSIT_H
