#ifndef CLIENT_DEBT_H
#define CLIENT_DEBT_H

#include "BankClasses.h"

class ClientDebt {
private:
    Number clientId;
    Number accountId;
    Number creditId;

public:
    ClientDebt(Number cId, Number aId, Number crId);
    
    Number getClientId() const;
    Number getAccountId() const;
    Number getCreditId() const;
};

#endif // CLIENT_DEBT_H