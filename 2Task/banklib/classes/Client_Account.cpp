#include "Client_Account.h"

ClientAccount::ClientAccount(Number cId, Number aId, Number dId) 
    : clientId(cId), accountId(aId), depositId(dId) {}

Number ClientAccount::getClientId() const { return clientId; }
Number ClientAccount::getAccountId() const { return accountId; }
Number ClientAccount::getDepositId() const { return depositId; }
