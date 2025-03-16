//
// Created by babadzakich on 15.03.2025.
//

#ifndef CREDIT_TYPE_H
#define CREDIT_TYPE_H

#include "Bank_enums.h"

enum class Credit_Type {
  CHARGED_DAILY,
  CHARGED_MONTHLY,
  CHARGED_QUARTERLY,
  CHARGED_SEMIANNUALLY,
  CHARGED_ANNUALLY,
};

Credit_Type stringToCreditType(std::string string);
std::string creditTypeToString(Credit_Type creditType);

#endif //CREDIT_TYPE_H
