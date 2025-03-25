#ifndef CLIENT_ACCOUNT_H
#define CLIENT_ACCOUNT_H

#include "banklib/Types.h"

class ClientDepositAccount {
 private:
  uint64_t clientId;
  uint64_t accountId;
  uint64_t depositId;

 public:
  ClientDepositAccount(uint64_t cId, uint64_t aId, uint64_t dId);
  ClientDepositAccount() : clientId(0), accountId(0), depositId(0) {};

  uint64_t getClientId() const;
  uint64_t getAccountId() const;
  uint64_t getDepositId() const;
};

#endif  // CLIENT_ACCOUNT_H