#pragma once

#include <banklib/Money.h>
#include <banklib/Types.h>
#include <banklib/enums/Credit_Type.h>
#include <banklib/enums/Currency_Type.h>

class CreditCardApplication {
 private:
  uint64_t clientId;
  Currency_Type currency;
  Credit_Type creditType;
  Money amount;
  Date applicationDate;

 public:
  CreditCardApplication()
      : clientId(0),
        currency(Currency_Type::RUB)  // or another default value
        ,
        creditType(Credit_Type::CHARGED_MONTHLY)  // or another default value
        ,
        amount(),
        applicationDate(0) {}

  CreditCardApplication(uint64_t clientId, Currency_Type currency, Credit_Type creditType, Money amount,
                        Date applicationDate)
      : clientId(clientId),
        currency(currency),
        creditType(creditType),
        amount(amount),
        applicationDate(applicationDate) {}

  uint64_t getClientId() const { return clientId; }
  Currency_Type getCurrency() const { return currency; }
  Credit_Type getCreditType() const { return creditType; }
  Money getAmount() const { return amount; }
  Date getApplicationDate() const { return applicationDate; }
};