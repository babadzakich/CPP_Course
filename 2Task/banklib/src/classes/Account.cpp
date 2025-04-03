//
// Created by babadzakich on 14.03.2025.
//

#include "banklib/classes/Account.h"

Account::Account(uint64_t id, Account_Type type, Money balance, Currency_Type currency, uint64_t clientId,
                 uint64_t associatedId)
    : id(id), type(type), balance(balance), currency(currency), clientId(clientId), associatedId(associatedId) {}

uint64_t Account::getId() const {
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

uint64_t Account::getClientId() const {
  return clientId;
}

uint64_t Account::getAssociatedId() const {
  return associatedId;
}
