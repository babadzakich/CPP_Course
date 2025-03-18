#include "banklib/classes/Bank_Client.h"

BankClient::BankClient(Number clientId, const std::string& name, Client_Type clientType)
    : clientId(clientId), name(name), clientType(clientType) {}

Number BankClient::getClientId() const {
  return clientId;
}

std::string BankClient::getName() const {
  return name;
}

Client_Type BankClient::getClientType() const {
  return clientType;
}
