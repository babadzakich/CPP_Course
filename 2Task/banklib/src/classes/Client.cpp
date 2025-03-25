#include "banklib/classes/Client.h"

Client::Client(uint64_t clientId, const std::string& name, Client_Type clientType)
    : clientId(clientId), name(name), clientType(clientType) {}

uint64_t Client::getClientId() const {
  return clientId;
}

std::string Client::getName() const {
  return name;
}

Client_Type Client::getClientType() const {
  return clientType;
}


std::set<accountID> Client::getCredits() const {
  return credits;
}

std::set<accountID> Client::getDeposits() const {
  return deposits;
}
