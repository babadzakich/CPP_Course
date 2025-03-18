#include "banklib/enums/Workspace_Type.h"

Workspace_Type stringToWorkspaceType(std::string string) {
  if (string == "Client Manager")
    return Workspace_Type::CLIENT_MANAGER;
  if (string == "Cash Desk")
    return Workspace_Type::CASH_DESK;
  if (string == "Currency Exchange")
    return Workspace_Type::CURRENCY_EXCHANGE;
  if (string == "VIP Client Manager")
    return Workspace_Type::VIP_CLIENT_MANAGER;
  throw std::invalid_argument("Invalid workspace type string");
}

std::string workspaceTypeToString(Workspace_Type workspace_type) {
  switch (workspace_type) {
    case Workspace_Type::CLIENT_MANAGER:
      return "Client Manager";
    case Workspace_Type::CASH_DESK:
      return "Cash Desk";
    case Workspace_Type::CURRENCY_EXCHANGE:
      return "Currency Exchange";
    case Workspace_Type::VIP_CLIENT_MANAGER:
      return "VIP Client Manager";
    default:
      throw std::invalid_argument("Invalid workspace type");
  }
}
