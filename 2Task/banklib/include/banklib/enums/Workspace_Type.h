#ifndef WORKSPACE_TYPE_H
#define WORKSPACE_TYPE_H

#include "banklib/enums/Bank_enums.h"

enum class Workspace_Type {
  CLIENT_MANAGER,
  CASH_DESK,
  CURRENCY_EXCHANGE,
  VIP_CLIENT_MANAGER,
};

Workspace_Type stringToWorkspaceType(std::string string);
std::string workspaceTypeToString(Workspace_Type workspace_type);

#endif
