#include "Exchange_Rate.h"

ExchangeRate::ExchangeRate(const Currency_Type& sell, const Currency_Type& buy, double rate) 
    : sellingCurrency_Type(sell), buyingCurrency_Type(buy), exchangeRate(rate) {}

Currency_Type ExchangeRate::getSellingCurrency() const { return sellingCurrency_Type; }

Currency_Type ExchangeRate::getBuyingCurrency() const { return buyingCurrency_Type; }

double ExchangeRate::getExchangeRate() const { return exchangeRate; }

