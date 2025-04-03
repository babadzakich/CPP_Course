#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include "banklib/Money.h"
#include "banklib/Types.h"

class BankAccount {
 private:
  uint64_t accountId;

 public:
  BankAccount() : accountId(0) {};
  BankAccount(uint64_t aId) : accountId(aId) {};

  uint64_t getAccountId() const;
};

#endif  // BANK_ACCOUNT_H