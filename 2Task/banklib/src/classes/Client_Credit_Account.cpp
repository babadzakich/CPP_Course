#include "banklib/classes/Client_Credit_Account.h"

ClientCreditAccount::ClientCreditAccount(uint64_t cId, uint64_t aId, uint64_t crId)
    : clientId(cId), accountId(aId), creditId(crId) {}

uint64_t ClientCreditAccount::getClientId() const {
  return clientId;
}

uint64_t ClientCreditAccount::getAccountId() const {
  return accountId;
}

uint64_t ClientCreditAccount::getCreditId() const {
  return creditId;
}
