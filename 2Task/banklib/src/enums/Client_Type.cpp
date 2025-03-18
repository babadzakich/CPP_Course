
#include "banklib/enums/Client_Type.h"

Client_Type stringToClientType(std::string string) {
  if (string == "Individual Client")
    return Client_Type::INDIVIDUAL_CLIENT;
  if (string == "Legal Entity")
    return Client_Type::LEGAL_ENTITY;
  if (string == "VIP Individual Client")
    return Client_Type::VIP_INDIVIDUAL_CLIENT;
  if (string == "VIP Legal Entity")
    return Client_Type::VIP_LEGAL_ENTITY;
  if (string == "Not a Client")
    return Client_Type::NOT_A_CLIENT;
  throw std::invalid_argument("Invalid client type string");
}

std::string clientTypeToString(Client_Type client_type) {
  switch (client_type) {
    case Client_Type::INDIVIDUAL_CLIENT:
      return "Individual Client";
    case Client_Type::LEGAL_ENTITY:
      return "Legal Entity";
    case Client_Type::VIP_INDIVIDUAL_CLIENT:
      return "VIP Individual Client";
    case Client_Type::VIP_LEGAL_ENTITY:
      return "VIP Legal Entity";
    case Client_Type::NOT_A_CLIENT:
      return "Not a Client";
    default:
      throw std::invalid_argument("Invalid client type");
  }
}
