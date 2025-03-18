//
// Created by babadzakich on 14.03.2025.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Account_Type.h"
#include "banklib/enums/Currency_Type.h"

class Account {
 private:
  Number id;
  Account_Type type;
  Money balance;
  Currency_Type currency;

 public:
  Account() : id(0), type(Account_Type::DEBIT), balance(Money(0, 0)), currency(Currency_Type::RUB) {};
  Account(Number id, Account_Type type, Money balance, Currency_Type currency);
  Number getId() const;
  Account_Type getType() const;
  Money getBalance() const;
  Currency_Type getCurrency() const;
  void takeMoney(Money amount);
  void putMoney(Money amount);
};

#endif  //ACCOUNT_H
