//
// Created by babadzakich on 14.03.2025.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "BankClasses.h"

class Account {
private:
    Number id;
    Account_Type type;
    Money balance;
    Currency_Type currency;
public:
    Account (Number id, Account_Type type, Money balance, Currency_Type currency);
    Number getId() const;
    Account_Type getType() const;
    Money getBalance() const;
    Currency_Type getCurrency() const;
    void takeMoney(Money amount);
    void putMoney(Money amount);
};

#endif //ACCOUNT_H
