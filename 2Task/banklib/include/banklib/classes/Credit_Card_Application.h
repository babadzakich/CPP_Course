#pragma once

#include <banklib/Types.h>
#include <banklib/enums/Credit_Type.h>
#include <banklib/enums/Currency_Type.h>
#include <banklib/Money.h>

class CreditCardApplication {
    private:
    uint64_t clientId;
    Currency_Type currency;
    Credit_Type creditType;
    Money amount;
    Date applicationDate;

    public:
    CreditCardApplication(uint64_t clientId, Currency_Type currency, Credit_Type creditType, Money amount, Date applicationDate)
        : clientId(clientId), currency(currency), creditType(creditType), amount(amount), applicationDate(applicationDate) {}
    CreditCardApplication() = default;

    uint64_t getClientId() const { return clientId; }
    Currency_Type getCurrency() const { return currency; }
    Credit_Type getCreditType() const { return creditType; }
    Money getAmount() const { return amount; }
    Date getApplicationDate() const { return applicationDate; }
  };