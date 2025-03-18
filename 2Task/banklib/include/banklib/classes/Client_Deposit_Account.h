#ifndef CLIENT_ACCOUNT_H
#define CLIENT_ACCOUNT_H

#include "banklib/Types.h"

class ClientDepositAccount {
 private:
  Number clientId;
  Number accountId;
  Number depositId;

 public:
  ClientDepositAccount(Number cId, Number aId, Number dId);
  ClientDepositAccount() : clientId(0), accountId(0), depositId(0) {};

  Number getClientId() const;
  Number getAccountId() const;
  Number getDepositId() const;
};

#endif  // CLIENT_ACCOUNT_H