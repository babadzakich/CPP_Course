#include "banklib/classes/Client_Deposit_Account.h"

ClientDepositAccount::ClientDepositAccount(Number cId, Number aId, Number dId)
    : clientId(cId), accountId(aId), depositId(dId) {}

Number ClientDepositAccount::getClientId() const {
  return clientId;
}
Number ClientDepositAccount::getAccountId() const {
  return accountId;
}
Number ClientDepositAccount::getDepositId() const {
  return depositId;
}
