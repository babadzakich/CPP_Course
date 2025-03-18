//
// Created by babadzakich on 14.03.2025.
//

#ifndef BANKCONTROLLER_H
#define BANKCONTROLLER_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/classes/BankClasses.h"
#include "banklib/enums/Bank_enums.h"

#include <string>
#include <utility>

class Time {
 private:
  int hours;
  int minutes;

 public:
  Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

  Time& operator++() {
    minutes++;
    hours += minutes / 60;
    minutes %= 60;
    hours %= 24;
    return *this;
  }

  Time operator++(int) {
    Time temp = *this;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
    }
    return temp;  // Return by value instead of reference
  }

  Time& operator+=(int minutes) {
    this->minutes += minutes;
    hours += this->minutes / 60;
    this->minutes %= 60;
    hours %= 24;
    return *this;
  }

  bool operator<(const Time& other) const {
    return hours < other.hours || (hours == other.hours && minutes < other.minutes);
  }

  bool operator>(const Time& other) const {
    return hours > other.hours || (hours == other.hours && minutes > other.minutes);
  }

  bool operator==(const Time& other) const { return hours == other.hours && minutes == other.minutes; }

  bool operator>=(const Time& other) const {
    return hours > other.hours || (hours == other.hours && minutes > other.minutes);
  }

  bool operator!=(const Time& other) const { return !(*this == other); }

  Number getHours() const { return hours; }

  Number getMinutes() const { return minutes; }
};
using Date = Number;

class bankController {
 private:
  Time currentTime = Time(0, 0);
  Date date = 0;

  BankClient clients[100000];
  Number clientCount;

  BankAccount bankAccountsId[100000];
  Number bankAccountCount;

  Account accounts[100000];
  Number accountCount;

  Credit credits[100000];
  Number creditCount;

  Deposit deposits[100000];
  Number depositCount;

  ExchangeRate exchangeRates[100000];
  Number exchangeRateCount;

  Workplace workplaces[100000];
  Number workplaceCount;

  ClientDepositAccount clientDepositAccounts[100000];
  Number clientDepositCount;

  ClientCreditAccount clientCreditAccounts[100000];
  Number clientCreditCount;

  Number openClientMoney[4][4] = {{10000, 4000, 25000, 15000},
                                  {2000, 1000, 5000, 2000},
                                  {100, 50, 200, 100},
                                  {100, 50, 200, 100}};

  Number maxClientAccountsAmount[4][4] = {{3, 5, 10, 25}, {1, 5, 5, 15}, {1, 3, 5, 10}, {1, 3, 5, 10}};

  // Loan interest rates by currency and client type (in percentage)
  double loanInterestRates[4][4] = {
      {12.2, 9.9, 16.7, 15.0},  // RUB
      {4.3, 4.0, 6.3, 5.8},     // YUAN
      {2.1, 1.5, 4.1, 3.6},     // USD
      {1.9, 1.1, 3.2, 3.1}      // EUR
  };

  // Credit card interest rates by currency and client type (in percentage)
  double creditCardInterestRates[4][4] = {
      {24.5, 21.2, 30.1, 29.7},  // RUB
      {16.0, 15.6, 20.1, 18.7},  // YUAN
      {9.0, 7.2, 13.4, 12.2},    // USD
      {8.5, 7.5, 14.7, 12.8}     // EUR
  };

  // Debit card interest rates by currency and client type (in percentage)
  double debitCardInterestRates[4][2][4] = {{
                                                // RUB
                                                {3.9, 5.3, 4.2, 6.2},  // Remaining balance
                                                {4.5, 5.7, 5.1, 6.7}   // Minimum balance
                                            },
                                            {// YUAN
                                             {2.1, 2.5, 3.2, 4.9},
                                             {2.4, 3.1, 3.7, 5.1}},
                                            {// USD
                                             {0.7, 1.0, 1.7, 2.1},
                                             {0.9, 1.2, 1.9, 2.3}},
                                            {// EUR
                                             {0.74, 0.9, 1.4, 1.95},
                                             {0.83, 1.15, 1.6, 2.3}}};

  const double FRACTIONAL_RESERVE_LIMIT = 0.8;  // 80% of deposits can be used

  Number loanLimits[4][4] = {{3000000, 15000000, 50000000, 100000000},
                             {600000, 1200000, 10000000, 25000000},
                             {15000, 95000, 600000, 1000000},
                             {20000, 100000, 750000, 1300000}};

  // Deposit interest rates by currency, duration and client type (in percentage)
  double depositInterestRates[4][4][4] = {{
                                              // RUB
                                              {9.0, 9.9, 0.0, 10.3},    // 3 months
                                              {9.7, 10.1, 0.0, 11.2},   // 6 months
                                              {11.5, 11.9, 0.0, 12.0},  // 12 months
                                              {12.1, 12.3, 0.0, 12.4}   // 24 months
                                          },
                                          {// YUAN
                                           {2.6, 3.2, 0.0, 2.9},
                                           {2.8, 3.5, 0.0, 3.4},
                                           {3.9, 3.8, 0.0, 3.7},
                                           {4.1, 4.0, 0.0, 4.2}},
                                          {// USD
                                           {1.3, 1.7, 0.0, 1.4},
                                           {1.5, 2.0, 0.0, 1.9},
                                           {1.8, 2.3, 0.0, 2.3},
                                           {2.0, 2.5, 0.0, 2.3}},
                                          {// EUR
                                           {0.7, 1.1, 0.0, 1.1},
                                           {1.1, 1.7, 0.0, 1.5},
                                           {1.5, 1.8, 0.0, 2.1},
                                           {1.9, 1.9, 0.0, 2.2}}};

  const Number DEPOSIT_DURATION_3M = 90;
  const Number DEPOSIT_DURATION_6M = 182;
  const Number DEPOSIT_DURATION_12M = 365;
  const Number DEPOSIT_DURATION_24M = 730;

  bool checkClientAccountCount(Number clientId, Currency_Type currency, Client_Type clientType);
  bool isClientExists(const std::string& name) const;
  bool canOperationBeHandledAtWorkplace(Operation_Type op, Workspace_Type workplace, Client_Type clientType) const;
  void registerNewClient(const std::string& name, Client_Type type);
  void client_getBalance(Number clientId, Number accountId);
  void client_openAccount(Number clientId, Money amount, Currency_Type currency, Client_Type clientType);
  void client_closeAccount(Number clientId, Number accountId);
  void client_withdrawFunds(Number clientId, Number accountId, Money amount);
  void client_depositFunds(Number clientId, Number accountId, Money amount);
  void client_getLoan(Number clientId, Currency_Type currency, Credit_Type creditType, Money amount);
  void client_requestCreditCard(Number clientId, Currency_Type currency, Credit_Type creditType, Money amount);
  void client_getCreditCard(Number clientId, Currency_Type currency, Credit_Type creditType, Money amount);
  void client_makeDeposit(Number clientId, Currency_Type currency, Deposit_Type depositType, Number duration,
                          Money amount);
  void client_requestDebitCard(Number clientId, Currency_Type currency, Deposit_Type depositType);
  void client_getDebitCard(Number clientId, Currency_Type currency, Deposit_Type depositType);
  void client_exchangeCurrency(Number clientId, Currency_Type fromCurrency, Currency_Type toCurrency, Money amount);

  // Add workplace statistics tracking
  struct WorkplaceStats {
    Number dailyClients;
    Number yearlyClients;
  };
  WorkplaceStats workplaceStats[100000];

  bool isWithinBankingHours(const Time& time) const { return time >= Time(8, 0) && time < Time(19, 0); }

  void handleTelephoneInquiry(const std::string& name, Number accountId);

  void handleOnlineCreditCardPayment(Number accountId, Currency_Type requestCurrency, Money amount);
  void handleOnlineDebitCardPayment(Number accountId, Currency_Type requestCurrency, Money amount);
  void handleOnlineTransferBetweenMyAccounts(Number fromAccountId, Number toAccountId, Money amount);
  void handleOnlineTransfer(Number fromAccountId, Number toAccountId, Money amount);
  void handleOnlineInboundTransfer(Number accountId, Money amount);

  // Monthly transfer limits for regular clients
  const Number MONTHLY_FREE_TRANSFER_LIMITS[4] = {
      100000,  // RUB
      10000,   // YUAN
      1000,    // USD
      1000     // EUR
  };

 public:
  bankController(const BankClient clients[], Number clientCount, const BankAccount bankAccountsId[],
                 Number bankAccountCount, const Account accounts[], Number accountCount, const Credit credits[],
                 Number creditCount, const Deposit deposits[], Number depositCount, const ExchangeRate exchangeRates[],
                 Number exchangeRateCount, const Workplace workplaces[], Number workplaceCount,
                 const ClientDepositAccount clientDepositAccounts[], Number clientDepositCount,
                 const ClientCreditAccount clientCreditAccounts[], Number clientCreditCount);

  void startBankDay(Date date, Time time);
  void endBankDay(Date date, Time time);
  void endBankMonth(Date date, Time time);
  void endBankQuarter(Date date, Time time);
  void endBankHalfYear(Date date, Time time);
  void endBankYear(Date date, Time time);

  void handleClientOperation(const std::string& name, Operation_Type* operations, Number countOperations);
  void handlePersonalAppeal(const std::string& name, Client_Type type, Operation_Type operation,
                            Number countOperations);
};

#endif  //BANKCONTROLLER_H
