//
// Created by babadzakich on 15.03.2025.
//

#include "banklib/enums/Account_Type.h"

Account_Type stringToAccountType(std::string accountType) {
  if (accountType == "debit")
    return Account_Type::DEBIT;
  if (accountType == "credit")
    return Account_Type::CREDIT;
  if (accountType == "deposit")
    return Account_Type::DEPOSIT;
  throw std::invalid_argument("Invalid account type");
}

std::string accountTypeToString(Account_Type accountType) {
  switch (accountType) {
    case Account_Type::DEBIT:
      return "debit";
    case Account_Type::CREDIT:
      return "credit";
    case Account_Type::DEPOSIT:
      return "deposit";
    default:
      throw std::invalid_argument("Invalid account type");
  }
}