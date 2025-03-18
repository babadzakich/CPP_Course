//
// Created by babadzakich on 15.03.2025.
//

#ifndef DEPOSIT_TYPE_H
#define DEPOSIT_TYPE_H

#include "banklib/enums/Bank_enums.h"

enum class Deposit_Type {
  COMPOUNDED_DAILY_REMAINING,
  COMPOUNDED_DAILY_MIN,
  COMPOUNDED_MONTHLY_REMAINING,
  COMPOUNDED_MONTHLY_MIN,
  COMPOUNDED_QUARTERLY_REMAINING,
  COMPOUNDED_QUARTERLY_MIN,
  COMPOUNDED_SEMIANUALLY_REMAINING,
  COMPOUNDED_SEMIANUALLY_MIN,
  COMPOUNDED_ANUALLY_REMAINING,
  COMPOUNDED_ANUALLY_MIN,
};

Deposit_Type stringToDepositType(const std::string& string);
std::string depositTypeToString(Deposit_Type type);

#endif  //DEPOSIT_TYPE_H
