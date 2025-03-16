//
// Created by babadzakich on 15.03.2025.
//

#include "Currency_Type.h"

Currency_Type stringToCurrency(std::string currency) {
    if (currency == "RUB") return Currency_Type::RUB;
    if (currency == "USD") return Currency_Type::USD;
    if (currency == "YUAN") return Currency_Type::YUAN;
    if (currency == "EUR") return Currency_Type::EUR;
    throw std::invalid_argument("Invalid currency");
}

std::string currencyToString(Currency_Type currency) {
    switch (currency) {
        case Currency_Type::RUB: return "RUB";
        case Currency_Type::USD: return "USD";
        case Currency_Type::YUAN: return "YUAN";
        case Currency_Type::EUR: return "EUR";
        default: throw std::invalid_argument("Invalid currency");
    }
}