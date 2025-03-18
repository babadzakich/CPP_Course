#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include "banklib/Money.h"
#include "banklib/Types.h"

class BankAccount {
 private:
  Number accountId;

 public:
  BankAccount() : accountId(0) {};
  BankAccount(Number aId) : accountId(aId) {};

  Number getAccountId() const;
};
;

#endif  // BANK_ACCOUNT_H