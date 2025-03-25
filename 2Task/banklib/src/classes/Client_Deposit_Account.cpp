#include "banklib/classes/Client_Deposit_Account.h"

ClientDepositAccount::ClientDepositAccount(uint64_t cId, uint64_t aId, uint64_t dId)
    : clientId(cId), accountId(aId), depositId(dId) {}

uint64_t ClientDepositAccount::getClientId() const {
  return clientId;
}
uint64_t ClientDepositAccount::getAccountId() const {
  return accountId;
}
uint64_t ClientDepositAccount::getDepositId() const {
  return depositId;
}
