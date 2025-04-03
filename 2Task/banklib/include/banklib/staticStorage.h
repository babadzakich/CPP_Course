#pragma once
#include <sys/types.h>
#include "banklib/Money.h"
#include "banklib/Types.h"

class StaticStorage {
 public:
  static const uint64_t openClientMoney[4][4];

  static const uint64_t maxClientAccountsAmount[4][4];

  static const double loanInterestRates[4][4];

  static const double creditCardInterestRates[4][4];

  static const double debitCardInterestRates[4][2][4];

  constexpr static const double FRACTIONAL_RESERVE_LIMIT = 0.8;

  static const uint64_t loanLimits[4][4];

  static const double depositInterestRates[4][4][4];

  static const uint64_t MONTHLY_FREE_TRANSFER_LIMITS[4];
};