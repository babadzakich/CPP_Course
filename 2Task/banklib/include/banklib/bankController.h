//
// Created by babadzakich on 14.03.2025.
//

#ifndef BANKCONTROLLER_H
#define BANKCONTROLLER_H

#include "banklib/classes/BankClasses.h"
#include "banklib/enums/Bank_enums.h"
#include "banklib/staticStorage.h"

#include <string>
#include <utility>
#include <vector>
#include <map>

class bankController {
 private:
  Time currentTime = Time(0, 0);
  Date date = 0;
  uint64_t accID = 0, clntID = 0, depID = 0, credID = 0;

  std::map<uint64_t, Client> clients;

  std::vector<BankAccount> bankAccountsId;

  std::map<uint64_t, Account> accounts;

  std::map<uint64_t, Credit> credits;

  std::map<uint64_t, Deposit> deposits;

  std::vector<ExchangeRate> exchangeRates;

  std::vector<Workplace> workplaces;

  bool checkClientAccountCount(uint64_t clientId, Currency_Type currency, Client_Type clientType);
  bool isClientExists(const std::string& name) const;
  bool canOperationBeHandledAtWorkplace(Operation_Type op, Workspace_Type workplace, Client_Type clientType) const;
  uint64_t registerNewClient(const std::string& name, Client_Type type);
  void client_getBalance(uint64_t clientId, uint64_t accountId);
  void client_openAccount(uint64_t clientId, Money amount, Currency_Type currency, Client_Type clientType);
  void client_closeAccount(uint64_t clientId, uint64_t accountId);
  void client_withdrawFunds(uint64_t clientId, uint64_t accountId, Money amount);
  void client_depositFunds(uint64_t clientId, uint64_t accountId, Money amount);
  void client_getLoan(uint64_t clientId, Currency_Type currency, Credit_Type creditType, Money amount);
  void client_requestCreditCard(uint64_t clientId, Currency_Type currency, Credit_Type creditType, Money amount);
  void client_getCreditCard(uint64_t clientId, Currency_Type currency, Credit_Type creditType, Money amount);
  void client_makeDeposit(uint64_t clientId, Currency_Type currency, Deposit_Type depositType, uint64_t duration,
                          Money amount);
  void client_requestDebitCard(uint64_t clientId, Currency_Type currency, Deposit_Type depositType);
  void client_getDebitCard(uint64_t clientId, Currency_Type currency, Deposit_Type depositType);
  void client_exchangeCurrency(uint64_t clientId, Currency_Type fromCurrency, Currency_Type toCurrency, Money amount);

  std::map<uint64_t, CreditCardApplication> pendingCreditCardApplications;
  std::map<uint64_t, DebitCardApplication> pendingDebitCardApplications;

  struct WorkplaceStats {
    uint64_t dailyClients;
    uint64_t yearlyClients;
  };
  std::vector<WorkplaceStats> workplaceStats;

  bool isWithinBankingHours(const Time& time) const { return time >= Time(8, 0) && time < Time(19, 0); }

  void handleTelephoneInquiry(const std::string& name, uint64_t accountId);

  void handleOnlineCreditCardPayment(uint64_t accountId, Currency_Type requestCurrency, Money amount);
  void handleOnlineDebitCardPayment(uint64_t accountId, Currency_Type requestCurrency, Money amount);
  void handleOnlineTransferBetweenMyAccounts(uint64_t fromAccountId, uint64_t toAccountId, Money amount);
  void handleOnlineTransfer(uint64_t fromAccountId, uint64_t toAccountId, Money amount);
  void handleOnlineInboundTransfer(uint64_t accountId, Money amount);

 public:
  bankController(const std::map<uint64_t, Client> clients, const std::vector<BankAccount> bankAccountsId,
                const std::map<uint64_t, Account> accounts, const std::map<uint64_t, Credit> credits, 
                const std::map<uint64_t, Deposit> deposits, const std::vector<ExchangeRate> exchangeRates, 
                const std::vector<Workplace> workplaces,
                const ClientDepositAccount clientDepositAccounts, 
                const ClientCreditAccount clientCreditAccounts);

  void startBankDay(Date date, Time time);
  void endBankDay(Date date, Time time);
  void endBankMonth(Date date, Time time);
  void endBankQuarter(Date date, Time time);
  void endBankHalfYear(Date date, Time time);
  void endBankYear(Date date, Time time);

  void handleClientOperation(uint64_t clientId, Operation_Type operation);
  void handlePersonalAppeal(const std::string& name, Client_Type type, std::vector<Operation_Type> operations);
};

#endif  //BANKCONTROLLER_H
