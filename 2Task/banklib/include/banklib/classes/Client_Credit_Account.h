#ifndef CLIENT_DEBT_H
#define CLIENT_DEBT_H

#include "banklib/Types.h"

class ClientCreditAccount {
 private:
  uint64_t clientId;
  uint64_t accountId;
  uint64_t creditId;

 public:
  ClientCreditAccount(uint64_t cId, uint64_t aId, uint64_t crId);
  ClientCreditAccount() : clientId(0), accountId(0), creditId(0) {};

  uint64_t getClientId() const;
  uint64_t getAccountId() const;
  uint64_t getCreditId() const;
};

#endif  // CLIENT_DEBT_H