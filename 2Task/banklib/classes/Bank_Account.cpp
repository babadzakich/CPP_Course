#include "Bank_Account.h"

BankAccount::BankAccount(Number aId) : accountId(aId) {}

Number BankAccount::getAccountId() const { return accountId; }

