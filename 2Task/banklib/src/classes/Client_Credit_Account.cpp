#include "banklib/classes/Client_Credit_Account.h"

ClientCreditAccount::ClientCreditAccount(Number cId, Number aId, Number crId)
    : clientId(cId), accountId(aId), creditId(crId) {}

Number ClientCreditAccount::getClientId() const {
  return clientId;
}

Number ClientCreditAccount::getAccountId() const {
  return accountId;
}

Number ClientCreditAccount::getCreditId() const {
  return creditId;
}
