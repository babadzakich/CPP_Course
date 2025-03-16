#ifndef EXCHANGE_RATE_H
#define EXCHANGE_RATE_H

#include "BankClasses.h"

class ExchangeRate {
private:
    Currency_Type sellingCurrency_Type;
    Currency_Type buyingCurrency_Type;
    double exchangeRate;

public:
    ExchangeRate(const Currency_Type& sell, const Currency_Type& buy, double rate);
    
    Currency_Type getSellingCurrency() const;
    Currency_Type getBuyingCurrency() const;
    double getExchangeRate() const;
};

#endif // EXCHANGE_RATE_H