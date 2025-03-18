//
// Created by babadzakich on 15.03.2025.
//

#include "banklib/enums/Credit_Type.h"

Credit_Type stringToCreditType(std::string string) {
  if (string == "Charged Daily")
    return Credit_Type::CHARGED_DAILY;
  if (string == "Charged Monthly")
    return Credit_Type::CHARGED_MONTHLY;
  if (string == "Charged Quarterly")
    return Credit_Type::CHARGED_QUARTERLY;
  if (string == "Charged Semi-Annually")
    return Credit_Type::CHARGED_SEMIANNUALLY;
  if (string == "Charged Annually")
    return Credit_Type::CHARGED_ANNUALLY;
  throw std::invalid_argument("Invalid Credit_Type");
}

std::string creditTypeToString(Credit_Type creditType) {
  switch (creditType) {
    case Credit_Type::CHARGED_DAILY:
      return "Charged Daily";
    case Credit_Type::CHARGED_MONTHLY:
      return "Charged Monthly";
    case Credit_Type::CHARGED_QUARTERLY:
      return "Charged Quarterly";
    case Credit_Type::CHARGED_SEMIANNUALLY:
      return "Charged Semi-Annually";
    case Credit_Type::CHARGED_ANNUALLY:
      return "Charged Annually";
    default:
      throw std::invalid_argument("Invalid Credit_Type");
  }
}