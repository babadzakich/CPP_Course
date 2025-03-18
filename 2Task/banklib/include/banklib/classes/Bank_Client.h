#ifndef BANK_CLIENT_H
#define BANK_CLIENT_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Client_Type.h"

class BankClient {
 private:
  Number clientId;
  std::string name;
  Client_Type clientType;

 public:
  BankClient() : clientId(0), name(""), clientType(Client_Type::INDIVIDUAL_CLIENT) {};
  BankClient(Number clientId, const std::string& name, Client_Type clientType);

  Number getClientId() const;
  std::string getName() const;
  Client_Type getClientType() const;
};

#endif  // BANK_CLIENT_H