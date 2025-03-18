//
// Created by babadzakich on 15.03.2025.
//

#ifndef CURRENCY_TYPE_H
#define CURRENCY_TYPE_H

#include "banklib/enums/Bank_enums.h"

enum class Currency_Type { RUB, YUAN, USD, EUR };

Currency_Type stringToCurrency(std::string currency);
std::string currencyToString(Currency_Type currency);

#endif  //CURRENCY_H
