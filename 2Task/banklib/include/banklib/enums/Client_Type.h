#ifndef CLIENT_TYPE_H
#define CLIENT_TYPE_H

#include "banklib/enums/Bank_enums.h"

enum class Client_Type { INDIVIDUAL_CLIENT, VIP_INDIVIDUAL_CLIENT, LEGAL_ENTITY, VIP_LEGAL_ENTITY, NOT_A_CLIENT };

std::string clientTypeToString(Client_Type clientType);
Client_Type stringToClientType(std::string string);

#endif