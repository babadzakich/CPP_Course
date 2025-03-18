//
// Created by babadzakich on 15.03.2025.
//

#ifndef BANK_ENUMS_H
#define BANK_ENUMS_H

#include <stdexcept>
#include <string>

#include "banklib/enums/Account_Type.h"
#include "banklib/enums/Client_Type.h"
#include "banklib/enums/Credit_Type.h"
#include "banklib/enums/Currency_Type.h"
#include "banklib/enums/Deposit_Type.h"
#include "banklib/enums/Workspace_Type.h"

enum class Operation_Type {
  CHECK_BALANCE,
  OPEN_ACCOUNT,
  CLOSE_ACCOUNT,
  WITHDRAW_MONEY,
  DEPOSIT_MONEY,
  GET_LOAN,
  APPLY_CREDIT_CARD,
  GET_CREDIT_CARD,
  OPEN_DEPOSIT,
  APPLY_DEBIT_CARD,
  GET_DEBIT_CARD,
  EXCHANGE_CURRENCY
};

#endif  //BANK_ENUMS_H
