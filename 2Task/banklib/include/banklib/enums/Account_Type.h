//
// Created by babadzakich on 15.03.2025.
//

#ifndef ACCOUNT_TYPE_H
#define ACCOUNT_TYPE_H

#include "banklib/enums/Bank_enums.h"

enum class Account_Type {
  DEBIT,
  CREDIT,
  DEPOSIT,
};

Account_Type stringToAccountType(std::string accountType);
std::string accountTypeToString(Account_Type accountType);

#endif  //ACCOUNT_TYPE_H
