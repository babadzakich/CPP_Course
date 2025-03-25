#ifndef BANK_CLIENT_H
#define BANK_CLIENT_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Client_Type.h"
using accountID = uint64_t;
using depositID = uint64_t;
using creditID = uint64_t;

class Client {
 private:
  uint64_t clientId;
  std::string name;
  Client_Type clientType;
  std::set<accountID> deposits;
  std::set<accountID> credits;
  
 public:
  Client() : clientId(0), name(""), clientType(Client_Type::INDIVIDUAL_CLIENT) {};
  Client(uint64_t clientId, const std::string& name, Client_Type clientType);

  uint64_t getClientId() const;
  std::string getName() const;
  Client_Type getClientType() const;
  
  std::set<accountID> getCredits() const;
  std::set<accountID> getDeposits() const;
};

#endif  // BANK_CLIENT_H