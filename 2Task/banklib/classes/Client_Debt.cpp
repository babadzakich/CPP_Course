#include "Client_Debt.h"

ClientDebt::ClientDebt(Number cId, Number aId, Number crId) 
    : clientId(cId), accountId(aId), creditId(crId) {}

Number ClientDebt::getClientId() const { return clientId; }

Number ClientDebt::getAccountId() const { return accountId; }

Number ClientDebt::getCreditId() const { return creditId; }

