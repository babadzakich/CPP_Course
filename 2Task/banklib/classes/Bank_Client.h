#ifndef BANK_CLIENT_H
#define BANK_CLIENT_H

#include "BankClasses.h"

class BankClient {
private:
    Number clientId;
    std::string name;
    Client_Type clientType;

public:
    BankClient() = default;
    BankClient(Number clientId, const std::string& name, Client_Type clientType);

    Number getClientId() const;
    std::string getName() const;
    Client_Type getClientType() const;
};

#endif // BANK_CLIENT_H