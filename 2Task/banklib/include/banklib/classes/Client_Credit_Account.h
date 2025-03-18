#ifndef CLIENT_DEBT_H
#define CLIENT_DEBT_H

#include "banklib/Types.h"

class ClientCreditAccount {
 private:
  Number clientId;
  Number accountId;
  Number creditId;

 public:
  ClientCreditAccount(Number cId, Number aId, Number crId);
  ClientCreditAccount() : clientId(0), accountId(0), creditId(0) {};

  Number getClientId() const;
  Number getAccountId() const;
  Number getCreditId() const;
};

#endif  // CLIENT_DEBT_H