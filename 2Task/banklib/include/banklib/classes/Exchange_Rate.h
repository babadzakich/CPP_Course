#ifndef EXCHANGE_RATE_H
#define EXCHANGE_RATE_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Currency_Type.h"

class ExchangeRate {
 private:
  Currency_Type sellingCurrency_Type;
  Currency_Type buyingCurrency_Type;
  double exchangeRate;

 public:
  ExchangeRate(const Currency_Type& sell, const Currency_Type& buy, double rate);
  ExchangeRate()
      : sellingCurrency_Type(Currency_Type::RUB), buyingCurrency_Type(Currency_Type::RUB), exchangeRate(1) {};

  Currency_Type getSellingCurrency() const;
  Currency_Type getBuyingCurrency() const;
  double getExchangeRate() const;
};

#endif  // EXCHANGE_RATE_H