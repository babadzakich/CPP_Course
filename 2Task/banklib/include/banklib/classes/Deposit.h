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
  Number id;
  double procent;
  Deposit_Type type;
  Date startDate;
  Number duration;

 public:
  Number getId() const;
  double getDailyProcent() const;
  double getMonthlyProcent() const;
  double getQuarterlyProcent() const;
  double getSemiannuallyProcent() const;
  double getAnnuallyProcent() const;
  Deposit_Type getType() const;
  Date getStartDate() const;
  Number getDuration() const;
  Deposit(Number id, double procent, Deposit_Type type, Date startDate, Number duration);
  Deposit() : id(0), procent(0), type(Deposit_Type::COMPOUNDED_ANUALLY_MIN), startDate(0), duration(0) {};
};

#endif  //DEPOSIT_H
