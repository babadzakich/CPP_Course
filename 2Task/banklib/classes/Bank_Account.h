#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include "BankClasses.h"

class BankAccount {
private:
    Number accountId;

public:
    BankAccount(Number aId);
    
    Number getAccountId() const;
};

#endif // BANK_ACCOUNT_H