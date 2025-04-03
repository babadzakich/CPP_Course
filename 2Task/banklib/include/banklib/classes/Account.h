//
// Created by babadzakich on 14.03.2025.
//

#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Account_Type.h"
#include "banklib/enums/Currency_Type.h"

#include <vector>

class Account {
 private:
  uint64_t id;
  Account_Type type;
  Money balance;
  Currency_Type currency;
  uint64_t clientId;
  uint64_t associatedId;

 public:
  Account()
      : id(0),
        type(Account_Type::DEBIT),
        balance(Money(0, 0)),
        currency(Currency_Type::RUB),
        clientId(0),
        associatedId(0) {};
  Account(uint64_t id, Account_Type type, Money balance, Currency_Type currency, uint64_t clientId,
          uint64_t associatedId);
  uint64_t getId() const;
  Account_Type getType() const;
  Money getBalance() const;
  Currency_Type getCurrency() const;
  void takeMoney(Money amount);
  void putMoney(Money amount);
  uint64_t getClientId() const;
  uint64_t getAssociatedId() const;
};

#endif  //ACCOUNT_H
