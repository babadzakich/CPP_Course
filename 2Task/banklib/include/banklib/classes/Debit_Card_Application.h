#pragma once

#include <banklib/Money.h>
#include <banklib/Types.h>
#include <banklib/enums/Currency_Type.h>
#include <banklib/enums/Deposit_Type.h>

class DebitCardApplication {
 private:
  uint64_t clientId;
  Currency_Type currency;
  Deposit_Type depositType;
  Date applicationDate;

 public:
  DebitCardApplication(uint64_t clientId, Currency_Type currency, Deposit_Type depositType, Date applicationDate)
      : clientId(clientId), currency(currency), depositType(depositType), applicationDate(applicationDate) {}
  DebitCardApplication() = default;

  uint64_t getClientId() const { return clientId; }
  Currency_Type getCurrency() const { return currency; }
  Deposit_Type getDepositType() const { return depositType; }
  Date getApplicationDate() const { return applicationDate; }
};