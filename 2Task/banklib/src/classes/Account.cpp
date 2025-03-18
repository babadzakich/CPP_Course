//
// Created by babadzakich on 14.03.2025.
//

#include "banklib/classes/Account.h"

Number Account::getId() const {
  return id;
}

Account_Type Account::getType() const {
  return type;
}

Money Account::getBalance() const {
  return balance;
}

Currency_Type Account::getCurrency() const {
  return currency;
}

void Account::takeMoney(Money amount) {
  if (balance < amount) {
    throw std::invalid_argument("Not enough money");
  }
  balance -= amount;
}

void Account::putMoney(Money amount) {
  balance += amount;
}

Account::Account(Number id, Account_Type type, Money balance, Currency_Type currency)
    : id(id), type(type), balance(balance), currency(currency) {}
