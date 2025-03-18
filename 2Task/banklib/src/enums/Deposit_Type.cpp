//
// Created by babadzakich on 15.03.2025.
//

#include "banklib/enums/Deposit_Type.h"

Deposit_Type stringToDepositType(const std::string& string) {
  if (string == "Compounded Daily Remaining")
    return Deposit_Type::COMPOUNDED_DAILY_REMAINING;
  if (string == "Compounded Daily Min")
    return Deposit_Type::COMPOUNDED_DAILY_MIN;
  if (string == "Compounded Monthly Remaining")
    return Deposit_Type::COMPOUNDED_MONTHLY_REMAINING;
  if (string == "Compounded Monthly Min")
    return Deposit_Type::COMPOUNDED_MONTHLY_MIN;
  if (string == "Compounded Quarterly Remaining")
    return Deposit_Type::COMPOUNDED_QUARTERLY_REMAINING;
  if (string == "Compounded Quarterly Min")
    return Deposit_Type::COMPOUNDED_QUARTERLY_MIN;
  if (string == "Compounded Semi-Annually Remaining")
    return Deposit_Type::COMPOUNDED_SEMIANUALLY_REMAINING;
  if (string == "Compounded Semi-Annually Min")
    return Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN;
  if (string == "Compounded Annually Remaining")
    return Deposit_Type::COMPOUNDED_ANUALLY_REMAINING;
  if (string == "Compounded Annually Min")
    return Deposit_Type::COMPOUNDED_ANUALLY_MIN;
  throw std::invalid_argument("Invalid deposit type");
}

std::string depositTypeToString(Deposit_Type type) {
  switch (type) {
    case Deposit_Type::COMPOUNDED_DAILY_REMAINING:
      return "Compounded Daily Remaining";
    case Deposit_Type::COMPOUNDED_DAILY_MIN:
      return "Compounded Daily Min";
    case Deposit_Type::COMPOUNDED_MONTHLY_REMAINING:
      return "Compounded Monthly Remaining";
    case Deposit_Type::COMPOUNDED_MONTHLY_MIN:
      return "Compounded Monthly Min";
    case Deposit_Type::COMPOUNDED_QUARTERLY_REMAINING:
      return "Compounded Quarterly Remaining";
    case Deposit_Type::COMPOUNDED_QUARTERLY_MIN:
      return "Compounded Quarterly Min";
    case Deposit_Type::COMPOUNDED_ANUALLY_REMAINING:
      return "Compounded Annually Remaining";
    case Deposit_Type::COMPOUNDED_ANUALLY_MIN:
      return "Compounded Annually Min";
    case Deposit_Type::COMPOUNDED_SEMIANUALLY_REMAINING:
      return "Compounded Semi-Annually Remaining";
    case Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN:
      return "Compounded Semi-Annually Min";
    default:
      throw std::invalid_argument("Invalid deposit type");
  }
}