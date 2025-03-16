#ifndef CLIENT_ACCOUNT_H
#define CLIENT_ACCOUNT_H

#include "BankClasses.h"

class ClientAccount {
private:
    Number clientId;
    Number accountId;
    Number depositId;

public:
    ClientAccount(Number cId, Number aId, Number dId);
    
    Number getClientId() const;
    Number getAccountId() const;
    Number getDepositId() const;
};

#endif // CLIENT_ACCOUNT_H