#include "banklib/bankController.h"

bankController::bankController(const BankClient clients[], Number clientCount, const BankAccount bankAccountsId[],
                               Number bankAccountCount, const Account accounts[], Number accountCount,
                               const Credit credits[], Number creditCount, const Deposit deposits[],
                               Number depositCount, const ExchangeRate exchangeRates[], Number exchangeRateCount,
                               const Workplace workplaces[], Number workplaceCount,
                               const ClientDepositAccount clientDepositAccounts[], Number clientDepositCount,
                               const ClientCreditAccount clientCreditAccounts[], Number clientCreditCount)
    : clientCount(clientCount),
      bankAccountCount(bankAccountCount),
      accountCount(accountCount),
      creditCount(creditCount),
      depositCount(depositCount),
      exchangeRateCount(exchangeRateCount),
      workplaceCount(workplaceCount),
      clientDepositCount(clientDepositCount),
      clientCreditCount(clientCreditCount) {
  for (int i = 0; i < clientCount; i++) {
    this->clients[i] = clients[i];
  }
  for (int i = 0; i < bankAccountCount; i++) {
    this->bankAccountsId[i] = bankAccountsId[i];
  }
  for (int i = 0; i < accountCount; i++) {
    this->accounts[i] = accounts[i];
  }
  for (int i = 0; i < creditCount; i++) {
    this->credits[i] = credits[i];
  }
  for (int i = 0; i < depositCount; i++) {
    this->deposits[i] = deposits[i];
  }
  for (int i = 0; i < exchangeRateCount; i++) {
    this->exchangeRates[i] = exchangeRates[i];
  }
  for (int i = 0; i < workplaceCount; i++) {
    this->workplaces[i] = workplaces[i];
  }
  for (int i = 0; i < clientDepositCount; i++) {
    this->clientDepositAccounts[i] = clientDepositAccounts[i];
  }
  for (int i = 0; i < clientCreditCount; i++) {
    this->clientCreditAccounts[i] = clientCreditAccounts[i];
  }
}

void bankController::startBankDay(Date day, Time time) {
  if (time != Time(8, 0)) {
    throw std::invalid_argument("Bank day can only start at 8:00");
  }
  currentTime = time;
  date = day;
  printf("%llu # %llu:%llu # Start of Bank Day", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankDay(Date day, Time time) {
  if (time != Time(20, 0)) {
    throw std::invalid_argument("Bank day can only end at 20:00");
  }

  for (int i = 0; i < clientCreditCount; i++) {
    Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
    if (credit.getType() == Credit_Type::CHARGED_DAILY) {
      Account& account = accounts[clientCreditAccounts[i].getAccountId()];
      for (int j = 0; j < bankAccountCount; j++) {
        if (account.getCurrency() == accounts[bankAccountsId[j].getAccountId()].getCurrency() &&
            account.getBalance() > 0) {
          Account& bankAccount = accounts[bankAccountsId[j].getAccountId()];
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal1(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal2());
          try {
            account.takeMoney(credit.getReceived() * credit.getDailyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Client Defaulted\n");
            exit(EXIT_FAILURE);
          }

          bankAccount.putMoney(credit.getReceived() * credit.getDailyProcent());
        }
      }
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    Deposit& deposit = deposits[clientDepositAccounts[i].getDepositId()];
    if (deposit.getType() == Deposit_Type::COMPOUNDED_DAILY_MIN ||
        deposit.getType() == Deposit_Type::COMPOUNDED_DAILY_REMAINING) {
      Account& account = accounts[clientDepositAccounts[i].getAccountId()];
      for (int k = 0; k < bankAccountCount; k++) {
        Account& bankAccount = accounts[bankAccountsId[k].getAccountId()];
        if (account.getCurrency() == bankAccount.getCurrency() && account.getBalance() > 0) {
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (account.getBalance() * deposit.getDailyProcent()).getVal1(),
                  (account.getBalance() * deposit.getDailyProcent()).getVal2());
          try {
            bankAccount.takeMoney(account.getBalance() * deposit.getDailyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Bank Defaulted\n");
            exit(EXIT_FAILURE);
          }

          account.putMoney(account.getBalance() * deposit.getDailyProcent());
        }
      }
    }
  }
  printf("%llu # %llu:%llu # End of Bank Day", date, currentTime.getHours(), currentTime.getMinutes());

  for (Number i = 0; i < workplaceCount; i++) {
    printf("%llu # %s # %llu # %llu\n", date, workspaceTypeToString(workplaces[i].getWorkplaceType()).c_str(), i + 1,
           workplaceStats[i].dailyClients);
    workplaceStats[i].dailyClients = 0;
  }
}

void bankController::endBankMonth(Date date, Time time) {
  for (int i = 0; i < clientCreditCount; i++) {
    Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
    if (credit.getType() == Credit_Type::CHARGED_MONTHLY) {
      Account& account = accounts[clientCreditAccounts[i].getAccountId()];
      for (int j = 0; j < bankAccountCount; j++) {
        if (account.getCurrency() == accounts[bankAccountsId[j].getAccountId()].getCurrency() &&
            account.getBalance() > 0) {
          Account& bankAccount = accounts[bankAccountsId[j].getAccountId()];
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal1(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal2());
          try {
            account.takeMoney(credit.getReceived() * credit.getMonthlyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Client Defaulted\n");
            exit(EXIT_FAILURE);
          }

          accounts[bankAccountsId[j].getAccountId()].putMoney(credit.getReceived() * credit.getMonthlyProcent());
        }
      }
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    Deposit& deposit = deposits[clientDepositAccounts[i].getDepositId()];
    if (deposit.getType() == Deposit_Type::COMPOUNDED_MONTHLY_MIN ||
        deposit.getType() == Deposit_Type::COMPOUNDED_MONTHLY_REMAINING) {
      Account& account = accounts[clientDepositAccounts[i].getAccountId()];
      for (int k = 0; k < bankAccountCount; k++) {
        Account& bankAccount = accounts[bankAccountsId[k].getAccountId()];
        if (account.getCurrency() == bankAccount.getCurrency() && account.getBalance() > 0) {
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (account.getBalance() * deposit.getMonthlyProcent()).getVal1(),
                  (account.getBalance() * deposit.getMonthlyProcent()).getVal2());
          try {
            bankAccount.takeMoney(account.getBalance() * deposit.getMonthlyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Bank Defaulted\n");
            exit(EXIT_FAILURE);
          }

          account.putMoney(account.getBalance() * deposit.getMonthlyProcent());
        }
      }
    }
  }

  printf("%llu # %llu:%llu # End of Bank Month", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankQuarter(Date date, Time time) {
  for (int i = 0; i < clientCreditCount; i++) {
    Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
    if (credit.getType() == Credit_Type::CHARGED_QUARTERLY) {
      Account& account = accounts[clientCreditAccounts[i].getAccountId()];
      for (int j = 0; j < bankAccountCount; j++) {
        Account& bankAccount = accounts[bankAccountsId[j].getAccountId()];
        fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                (credit.getReceived() * credit.getDailyProcent()).getVal1(),
                (credit.getReceived() * credit.getDailyProcent()).getVal2());
        if (account.getCurrency() == accounts[bankAccountsId[j].getAccountId()].getCurrency() &&
            account.getBalance() > 0) {
          try {
            account.takeMoney(credit.getReceived() * credit.getQuarterlyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Client Defaulted\n");
            exit(EXIT_FAILURE);
          }

          accounts[bankAccountsId[j].getAccountId()].putMoney(credit.getReceived() * credit.getQuarterlyProcent());
        }
      }
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    Deposit& deposit = deposits[clientDepositAccounts[i].getDepositId()];
    if (deposit.getType() == Deposit_Type::COMPOUNDED_QUARTERLY_MIN ||
        deposit.getType() == Deposit_Type::COMPOUNDED_QUARTERLY_REMAINING) {
      Account& account = accounts[clientDepositAccounts[i].getAccountId()];
      for (int k = 0; k < bankAccountCount; k++) {
        Account& bankAccount = accounts[bankAccountsId[k].getAccountId()];
        if (account.getCurrency() == bankAccount.getCurrency() && account.getBalance() > 0) {
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (account.getBalance() * deposit.getQuarterlyProcent()).getVal1(),
                  (account.getBalance() * deposit.getQuarterlyProcent()).getVal2());
          try {
            bankAccount.takeMoney(account.getBalance() * deposit.getQuarterlyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Bank Defaulted\n");
            exit(EXIT_FAILURE);
          }

          account.putMoney(account.getBalance() * deposit.getQuarterlyProcent());
        }
      }
    }
  }

  printf("%llu # %llu:%llu # End of Bank Quarter", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankHalfYear(Date date, Time time) {
  for (int i = 0; i < clientCreditCount; i++) {
    Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
    if (credit.getType() == Credit_Type::CHARGED_SEMIANNUALLY) {
      Account& account = accounts[clientCreditAccounts[i].getAccountId()];
      for (int j = 0; j < bankAccountCount; j++) {
        if (account.getCurrency() == accounts[bankAccountsId[j].getAccountId()].getCurrency() &&
            account.getBalance() > 0) {
          Account& bankAccount = accounts[bankAccountsId[j].getAccountId()];
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal1(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal2());
          try {
            account.takeMoney(credit.getReceived() * credit.getSemiannualProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Bank Defaulted\n");
            exit(EXIT_FAILURE);
          }

          accounts[bankAccountsId[j].getAccountId()].putMoney(credit.getReceived() * credit.getSemiannualProcent());
        }
      }
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    Deposit& deposit = deposits[clientDepositAccounts[i].getDepositId()];
    if (deposit.getType() == Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN ||
        deposit.getType() == Deposit_Type::COMPOUNDED_SEMIANUALLY_REMAINING) {
      Account& account = accounts[clientDepositAccounts[i].getAccountId()];
      for (int k = 0; k < bankAccountCount; k++) {
        Account& bankAccount = accounts[bankAccountsId[k].getAccountId()];
        if (account.getCurrency() == bankAccount.getCurrency() && account.getBalance() > 0) {
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (account.getBalance() * deposit.getSemiannuallyProcent()).getVal1(),
                  (account.getBalance() * deposit.getSemiannuallyProcent()).getVal2());
          try {
            bankAccount.takeMoney(account.getBalance() * deposit.getSemiannuallyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Bank Defaulted\n");
            exit(EXIT_FAILURE);
          }

          account.putMoney(account.getBalance() * deposit.getSemiannuallyProcent());
        }
      }
    }
  }

  printf("%llu # %llu:%llu # End of Bank Semi-Annual period", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankYear(Date date, Time time) {
  for (int i = 0; i < clientCreditCount; i++) {
    Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
    if (credit.getType() == Credit_Type::CHARGED_ANNUALLY) {
      Account& account = accounts[clientCreditAccounts[i].getAccountId()];
      for (int j = 0; j < bankAccountCount; j++) {
        if (account.getCurrency() == accounts[bankAccountsId[j].getAccountId()].getCurrency() &&
            account.getBalance() > 0) {
          Account& bankAccount = accounts[bankAccountsId[j].getAccountId()];
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal1(),
                  (credit.getReceived() * credit.getDailyProcent()).getVal2());
          try {
            account.takeMoney(credit.getReceived() * credit.getAnnualProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Bank Defaulted\n");
            exit(EXIT_FAILURE);
          }

          accounts[bankAccountsId[j].getAccountId()].putMoney(credit.getReceived() * credit.getAnnualProcent());
        }
      }
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    Deposit& deposit = deposits[clientDepositAccounts[i].getDepositId()];
    if (deposit.getType() == Deposit_Type::COMPOUNDED_ANUALLY_MIN ||
        deposit.getType() == Deposit_Type::COMPOUNDED_ANUALLY_REMAINING) {
      Account& account = accounts[clientDepositAccounts[i].getAccountId()];
      for (int k = 0; k < bankAccountCount; k++) {
        Account& bankAccount = accounts[bankAccountsId[k].getAccountId()];
        if (account.getCurrency() == bankAccount.getCurrency() && account.getBalance() > 0) {
          fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                  currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                  (account.getBalance() * deposit.getAnnuallyProcent()).getVal1(),
                  (account.getBalance() * deposit.getAnnuallyProcent()).getVal2());
          try {
            bankAccount.takeMoney(account.getBalance() * deposit.getAnnuallyProcent());
          } catch (const std::invalid_argument& e) {
            fprintf(stderr, "Bank Defaulted\n");
            exit(EXIT_FAILURE);
          }

          account.putMoney(account.getBalance() * deposit.getAnnuallyProcent());
        }
      }
    }
  }
  printf("%llu # %llu:%llu # End of Bank Year", date, currentTime.getHours(), currentTime.getMinutes());

  for (Number i = 0; i < workplaceCount; i++) {
    printf("%llu # %s # %llu # %llu\n", date, workspaceTypeToString(workplaces[i].getWorkplaceType()).c_str(), i + 1,
           workplaceStats[i].yearlyClients);
    workplaceStats[i].yearlyClients = 0;
  }
}

void bankController::registerNewClient(const std::string& name, Client_Type type) {
  if (isClientExists(name)) {
    throw std::invalid_argument("Client already exists");
  }
}

bool bankController::isClientExists(const std::string& name) const {
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getName() == name) {
      return true;
    }
  }
  return false;
}

bool bankController::canOperationBeHandledAtWorkplace(Operation_Type op, Workspace_Type workplace,
                                                      Client_Type clientType) const {
  switch (op) {
    case Operation_Type::CHECK_BALANCE:
      return workplace == Workspace_Type::CASH_DESK || workplace == Workspace_Type::CLIENT_MANAGER;
    case Operation_Type::OPEN_ACCOUNT:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::CLOSE_ACCOUNT:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::WITHDRAW_MONEY:
      return workplace == Workspace_Type::CASH_DESK;
    case Operation_Type::DEPOSIT_MONEY:
      return workplace == Workspace_Type::CASH_DESK;
    case Operation_Type::GET_LOAN:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::APPLY_CREDIT_CARD:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::GET_CREDIT_CARD:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::OPEN_DEPOSIT:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::APPLY_DEBIT_CARD:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::GET_DEBIT_CARD:
      return (workplace == Workspace_Type::CLIENT_MANAGER && clientType != Client_Type::NOT_A_CLIENT) ||
             (workplace == Workspace_Type::VIP_CLIENT_MANAGER && clientType == Client_Type::VIP_INDIVIDUAL_CLIENT);
    case Operation_Type::EXCHANGE_CURRENCY:
      return workplace == Workspace_Type::CURRENCY_EXCHANGE;
    default:
      return false;
  }
}

void bankController::handlePersonalAppeal(const std::string& name, Client_Type type, Operation_Type operation,
                                          Number countOperations) {
  BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getName() == name) {
      client = &clients[i];
      break;
    }
  }

  if (client == nullptr) {
    if (operation != Operation_Type::OPEN_ACCOUNT && operation != Operation_Type::APPLY_DEBIT_CARD) {
      throw std::runtime_error("Client error. Wrong operation for new client");
    }
    currentTime += 15;
    registerNewClient(name, type);
  }
}

void bankController::handleClientOperation(const std::string& name, Operation_Type* operations,
                                           Number countOperations) {
  if (!isWithinBankingHours(currentTime)) {
    if (currentTime < Time(8, 0)) {
      throw std::runtime_error("Bank closed until 8:00");
    } else {
      throw std::runtime_error("Bank is closing at 19:00");
    }
  }

  BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getName() == name) {
      client = &clients[i];
      break;
    }
  }

  for (Number i = 0; i < workplaceCount; i++) {
    workplaceStats[i].dailyClients++;
    workplaceStats[i].yearlyClients++;
  }
}

void bankController::client_getBalance(Number clientId, Number accountId) {
  currentTime += 5;
  for (int i = 0; i < clientCreditCount; i++) {
    if (clientCreditAccounts[i].getAccountId() == accountId) {
      if (clientCreditAccounts[i].getClientId() == clientId) {
        printf("%llu # %llu:%llu # Balance of %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
               accountId, accounts[clientId].getBalance().getVal1(), accounts[clientId].getBalance().getVal2());
        return;
      }
      throw std::invalid_argument("Client error. Access denied");
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    if (clientDepositAccounts[i].getAccountId() == accountId) {
      if (clientDepositAccounts[i].getClientId() == clientId) {
        printf("%llu # %llu:%llu # Balance of %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
               accountId, accounts[clientId].getBalance().getVal1(), accounts[clientId].getBalance().getVal2());
        return;
      }
      throw std::invalid_argument("Client error. Access denied");
    }
  }
  throw std::invalid_argument("Client error. Unknown account");
  if (currentTime >= Time(19, 00))
    bankController::endBankDay(date, currentTime);
}

bool bankController::checkClientAccountCount(Number clientId, Currency_Type currency, Client_Type clientType) {
  int count = 0;
  for (int i = 0; i < clientCreditCount; i++) {
    if (clientCreditAccounts[i].getClientId() == clientId)
      count++;
    if (count >= maxClientAccountsAmount[(Number)currency][(Number)clientType]) {
      return false;
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    if (clientDepositAccounts[i].getClientId() == clientId)
      count++;
    if (count >= maxClientAccountsAmount[(Number)currency][(Number)clientType]) {
      return false;
    }
  }
  return true;
}

void bankController::client_openAccount(Number clientId, Money amount, Currency_Type currency, Client_Type clientType) {
  if (!checkClientAccountCount(clientId, currency, clientType))
    throw std::invalid_argument("Client error. Active account limit reached");

  currentTime += 10;
  Account account(accountCount, Account_Type::DEPOSIT, Money(-openClientMoney[(int)currency][(int)clientType], 0),
                  currency);
  accounts[accountCount++] = account;
  Deposit deposit(depositCount, 0.0, Deposit_Type::COMPOUNDED_ANUALLY_MIN, date, 0);
  deposits[depositCount++] = deposit;
  ClientDepositAccount depAcc(clientId, account.getId(), deposit.getId());
  clientDepositAccounts[clientDepositCount++] = depAcc;
}

void bankController::client_closeAccount(Number clientId, Number accountId) {
  currentTime += 25;

  bool found = false;
  Account* account = nullptr;
  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].getId() == accountId) {
      account = &accounts[i];
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::invalid_argument("Client error. Unknown account");
  }

  bool isOwner = false;
  for (int i = 0; i < clientDepositCount && !isOwner; i++) {
    if (clientDepositAccounts[i].getAccountId() == accountId && clientDepositAccounts[i].getClientId() == clientId) {
      isOwner = true;
    }
  }
  for (int i = 0; i < clientCreditCount && !isOwner; i++) {
    if (clientCreditAccounts[i].getAccountId() == accountId && clientCreditAccounts[i].getClientId() == clientId) {
      isOwner = true;
    }
  }
  if (!isOwner) {
    throw std::invalid_argument("Client error. Access denied");
  }

  if (account->getBalance() < Money(0, 0)) {
    throw std::invalid_argument("Client error. Negative balance on closure");
  }

  for (int i = 0; i < clientCreditCount; i++) {
    if (clientCreditAccounts[i].getAccountId() == accountId) {
      Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
      if (credit.getReceived() > account->getBalance()) {
        throw std::invalid_argument("Credit in action");
      }
      printf("%llu # %llu:%llu # Account closed %llu # %llu.%llu", date, currentTime.getHours(),
             currentTime.getMinutes(), accountId, account->getBalance().getVal1(), account->getBalance().getVal2());
      return;
    }
  }

  Money fee = account->getBalance() * 0.015;
  if (fee < Money(10, 0)) {
    fee = Money(10, 0);
  }

  Money finalAmount = account->getBalance() - fee;
  printf("%llu # %llu:%llu # Account closed %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
         accountId, finalAmount.getVal1(), finalAmount.getVal2());

  if (currentTime >= Time(19, 00)) {
    bankController::endBankDay(date, currentTime);
  }
}

void bankController::client_withdrawFunds(Number clientId, Number accountId, Money amount) {
  currentTime += 10;

  bool found = false;
  Account* account = nullptr;
  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].getId() == accountId) {
      account = &accounts[i];
      found = true;
      break;
    }
  }

  if (!found) {
    throw std::invalid_argument("Client error. Unknown account");
  }

  Deposit deposit;
  bool isOwner = false, creditOrDebit = false;
  for (int i = 0; i < clientDepositCount && !isOwner; i++) {
    if (clientDepositAccounts[i].getAccountId() == accountId && clientDepositAccounts[i].getClientId() == clientId) {
      isOwner = true;
      creditOrDebit = true;
      for (int j = 0; j < depositCount; j++) {
        if (deposits[j].getId() == clientDepositAccounts[i].getDepositId()) {
          deposit = deposits[j];
          break;
        }
      }
    }
  }
  for (int i = 0; i < clientCreditCount && !isOwner; i++) {
    if (clientCreditAccounts[i].getAccountId() == accountId && clientCreditAccounts[i].getClientId() == clientId) {
      isOwner = true;
    }
  }
  if (!isOwner) {
    throw std::invalid_argument("Client error. Access denied");
  }

  Money fee(0, 0);

  if (creditOrDebit && deposit.getDuration() * 30 <= date - deposit.getStartDate()) {
    fee = amount * 0.0025;
    if (fee < Money(2, 50)) {
      fee = Money(2, 50);
    }
  }

  if (account->getBalance() < (amount + fee)) {

    Money totalDeposits(0, 0);
    for (int i = 0; i < clientDepositCount; i++) {
      Account& depositAccount = accounts[clientDepositAccounts[i].getAccountId()];
      if (depositAccount.getCurrency() == account->getCurrency()) {
        totalDeposits += depositAccount.getBalance();
      }
    }

    Money maxReserveUse = totalDeposits * FRACTIONAL_RESERVE_LIMIT;
    if (maxReserveUse >= (amount + fee - account->getBalance())) {

      for (int i = 0; i < clientDepositCount && amount + fee > account->getBalance(); i++) {
        Account& depositAccount = accounts[clientDepositAccounts[i].getAccountId()];
        if (depositAccount.getCurrency() == account->getCurrency()) {
          Money reserveAmount = depositAccount.getBalance() * FRACTIONAL_RESERVE_LIMIT;
          depositAccount.takeMoney(reserveAmount);
          account->putMoney(reserveAmount);
        }
      }
    }
  }

  if (account->getBalance() < (amount + fee)) {
    throw std::invalid_argument("Client error. Insufficient funds");
  }

  account->takeMoney(amount + fee);

  printf("%llu # %llu:%llu # Funds Withdrawn %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
         accountId, account->getBalance().getVal1(), account->getBalance().getVal2());

  if (currentTime >= Time(19, 00)) {
    bankController::endBankDay(date, currentTime);
  }
}

void bankController::client_depositFunds(Number clientId, Number accountId, Money amount) {
  currentTime += 5;

  bool found = false;
  Account* account = nullptr;
  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].getId() == accountId) {
      account = &accounts[i];
      found = true;
      break;
    }
  }
  if (!found) {
    throw std::invalid_argument("Client error. Unknown account");
  }

  bool isOwner = false;
  bool isLoanAccount = false;
  Credit* credit = nullptr;

  for (int i = 0; i < clientCreditCount && !isOwner; i++) {
    if (clientCreditAccounts[i].getAccountId() == accountId) {
      if (clientCreditAccounts[i].getClientId() == clientId) {
        isOwner = true;
        isLoanAccount = true;
        credit = &credits[clientCreditAccounts[i].getCreditId()];
      } else {
        throw std::invalid_argument("Client error. Access denied");
      }
    }
  }

  for (int i = 0; i < clientDepositCount && !isOwner; i++) {
    if (clientDepositAccounts[i].getAccountId() == accountId) {
      if (clientDepositAccounts[i].getClientId() == clientId) {
        isOwner = true;
      } else {
        throw std::invalid_argument("Client error. Access denied");
      }
    }
  }

  if (!isOwner) {
    throw std::invalid_argument("Client error. Access denied");
  }

  Money fee = amount * 0.0001;
  if (fee < Money(1, 0)) {
    fee = Money(1, 0);
  }

  if (isLoanAccount && (account->getBalance() + amount - fee) > credit->getReceived()) {
    throw std::invalid_argument("Client error. Invalid loan payment amount");
  }

  account->putMoney(amount - fee);

  printf("%llu # %llu:%llu # Fonds Credited %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
         accountId, account->getBalance().getVal1(), account->getBalance().getVal2());

  if (currentTime >= Time(19, 00)) {
    bankController::endBankDay(date, currentTime);
  }
}

void bankController::client_getLoan(Number clientId, Currency_Type currency, Credit_Type creditType, Money amount) {
  currentTime += 45;

  const BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getClientId() == clientId) {
      client = &clients[i];
      break;
    }
  }
  if (!client) {
    throw std::invalid_argument("Client not found");
  }

  if (amount.getValue() > loanLimits[(int)currency][(int)client->getClientType()]) {
    throw std::invalid_argument("Client error. Loan amount exceeds limit");
  }

  bool hasSufficientFunds = false;
  for (int i = 0; i < bankAccountCount; i++) {
    Account& bankAccount = accounts[bankAccountsId[i].getAccountId()];
    if (bankAccount.getCurrency() == currency && bankAccount.getBalance() >= amount) {
      hasSufficientFunds = true;
      bankAccount.takeMoney(amount);
      break;
    }
  }

  if (!hasSufficientFunds) {
    throw std::runtime_error("Can't process credit");
  }

  Money fee = amount * 0.001;
  Account creditAccount(accountCount, Account_Type::CREDIT, Money(0, 0) - fee, currency);

  Credit credit(creditCount, loanInterestRates[(int)currency][(int)client->getClientType()], creditType, amount);
  credits[creditCount++] = credit;

  ClientCreditAccount clientCredit(clientId, creditAccount.getId(), credit.getId());
  clientCreditAccounts[clientCreditCount++] = clientCredit;

  printf("%llu # %llu:%llu # Loan Approved %llu", date, currentTime.getHours(), currentTime.getMinutes(),
         creditAccount.getId());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::client_requestCreditCard(Number clientId, Currency_Type currency, Credit_Type creditType,
                                              Money amount) {
  currentTime += 10;

  const BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getClientId() == clientId) {
      client = &clients[i];
      break;
    }
  }
  if (!client) {
    throw std::invalid_argument("Client not found");
  }

  if (amount.getValue() > loanLimits[(int)currency][(int)client->getClientType()]) {
    throw std::invalid_argument("Client error. Credit amount exceeds limit");
  }

  printf("%llu # %llu:%llu # Credit Card Application Received", date, currentTime.getHours(), currentTime.getMinutes());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::client_getCreditCard(Number clientId, Currency_Type currency, Credit_Type creditType,
                                          Money amount) {
  currentTime += 5;

  const BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getClientId() == clientId) {
      client = &clients[i];
      break;
    }
  }
  if (!client) {
    throw std::invalid_argument("Client not found");
  }

  bool applicationFound = false;
  Date applicationDate = 0;
  for (Number i = 0; i < clientCreditCount; i++) {

    if (clientCreditAccounts[i].getClientId() == clientId) {
      Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
      if (credit.getReceived() == amount &&
          accounts[clientCreditAccounts[i].getAccountId()].getCurrency() == currency) {
        applicationFound = true;
        applicationDate = date;
        break;
      }
    }
  }

  if (!applicationFound) {
    throw std::invalid_argument("Client error. No matching credit card application found");
  }

  Number daysSinceApplication = date - applicationDate;
  if (daysSinceApplication < 3) {
    throw std::invalid_argument("Client error. Credit Card application not yet approved");
  }
  if (daysSinceApplication > 30) {
    throw std::invalid_argument("Client error. Credit Card application out of date");
  }

  bool hasSufficientFunds = false;
  for (int i = 0; i < bankAccountCount; i++) {
    Account& bankAccount = accounts[bankAccountsId[i].getAccountId()];
    if (bankAccount.getCurrency() == currency && bankAccount.getBalance() >= amount) {
      hasSufficientFunds = true;
      bankAccount.takeMoney(amount);
      break;
    }
  }

  if (!hasSufficientFunds) {
    throw std::runtime_error("Can't process credit card");
  }

  Money openingFee = Money(-openClientMoney[(int)currency][(int)client->getClientType()], 0);

  Account creditCardAccount(accountCount, Account_Type::CREDIT, amount + openingFee, currency);
  accounts[accountCount++] = creditCardAccount;

  Credit credit(creditCount, creditCardInterestRates[(int)currency][(int)client->getClientType()], creditType, amount);
  credits[creditCount++] = credit;

  ClientCreditAccount clientCredit(clientId, creditCardAccount.getId(), credit.getId());
  clientCreditAccounts[clientCreditCount++] = clientCredit;

  printf("%llu # %llu:%llu # Credit Card Approved # %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), creditCardAccount.getId(), creditCardAccount.getBalance().getVal1(),
         creditCardAccount.getBalance().getVal2());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::client_makeDeposit(Number clientId, Currency_Type currency, Deposit_Type depositType,
                                        Number duration, Money amount) {
  currentTime += 20;

  const BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getClientId() == clientId) {
      client = &clients[i];
      break;
    }
  }
  if (!client) {
    throw std::invalid_argument("Client not found");
  }

  int durationIdx;
  if (duration == DEPOSIT_DURATION_3M)
    durationIdx = 0;
  else if (duration == DEPOSIT_DURATION_6M)
    durationIdx = 1;
  else if (duration == DEPOSIT_DURATION_12M)
    durationIdx = 2;
  else if (duration == DEPOSIT_DURATION_24M)
    durationIdx = 3;
  else
    throw std::invalid_argument("Invalid deposit duration");

  if (client->getClientType() == Client_Type::LEGAL_ENTITY) {
    throw std::invalid_argument("Client error. Legal entities cannot open deposits");
  }

  double interestRate = depositInterestRates[(int)currency][durationIdx][(int)client->getClientType()];
  if (interestRate == 0.0) {
    throw std::invalid_argument("Client error. Invalid deposit parameters");
  }

  Account depositAccount(accountCount, Account_Type::DEPOSIT, amount, currency);
  accounts[accountCount++] = depositAccount;

  Deposit deposit(depositCount, interestRate, depositType, date, duration);
  deposits[depositCount++] = deposit;

  ClientDepositAccount clientDeposit(clientId, depositAccount.getId(), deposit.getId());
  clientDepositAccounts[clientDepositCount++] = clientDeposit;

  printf("%llu # %llu:%llu # Deposit Approved # %llu", date, currentTime.getHours(), currentTime.getMinutes(),
         depositAccount.getId());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::client_requestDebitCard(Number clientId, Currency_Type currency, Deposit_Type depositType) {
  currentTime += 10;

  const BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getClientId() == clientId) {
      client = &clients[i];
      break;
    }
  }
  if (!client) {
    throw std::invalid_argument("Client not found");
  }

  bool isMinimumType =
      (depositType == Deposit_Type::COMPOUNDED_DAILY_MIN || depositType == Deposit_Type::COMPOUNDED_MONTHLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_QUARTERLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN || depositType == Deposit_Type::COMPOUNDED_ANUALLY_MIN);

  printf("%llu # %llu:%llu # Debit Card Application Received", date, currentTime.getHours(), currentTime.getMinutes());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::client_getDebitCard(Number clientId, Currency_Type currency, Deposit_Type depositType) {
  currentTime += 5;

  const BankClient* client = nullptr;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getClientId() == clientId) {
      client = &clients[i];
      break;
    }
  }
  if (!client) {
    throw std::invalid_argument("Client not found");
  }

  bool applicationFound = false;
  Date applicationDate = 0;
  for (Number i = 0; i < clientDepositCount; i++) {
    if (clientDepositAccounts[i].getClientId() == clientId) {
      Deposit& deposit = deposits[clientDepositAccounts[i].getDepositId()];
      Account& account = accounts[clientDepositAccounts[i].getAccountId()];
      if (deposit.getType() == depositType && account.getCurrency() == currency) {
        applicationFound = true;
        applicationDate = deposit.getStartDate();
        break;
      }
    }
  }

  if (!applicationFound) {
    throw std::invalid_argument("Client error. No matching debit card application found");
  }

  Number daysSinceApplication = date - applicationDate;
  if (daysSinceApplication < 5) {
    throw std::invalid_argument("Client error. Debit Card application not yet approved");
  }
  if (daysSinceApplication > 15) {
    throw std::invalid_argument("Client error. Debit Card application out of date");
  }

  Money openingFee = Money(-openClientMoney[(int)currency][(int)client->getClientType()], 0);

  Account debitAccount(accountCount, Account_Type::DEPOSIT, openingFee, currency);
  accounts[accountCount++] = debitAccount;

  bool isMinimumType =
      (depositType == Deposit_Type::COMPOUNDED_DAILY_MIN || depositType == Deposit_Type::COMPOUNDED_MONTHLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_QUARTERLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN || depositType == Deposit_Type::COMPOUNDED_ANUALLY_MIN);

  Deposit deposit(depositCount,
                  debitCardInterestRates[(int)currency][isMinimumType ? 1 : 0][(int)client->getClientType()],
                  depositType, date, 0);
  deposits[depositCount++] = deposit;

  ClientDepositAccount clientDeposit(clientId, debitAccount.getId(), deposit.getId());
  clientDepositAccounts[clientDepositCount++] = clientDeposit;

  printf("%llu # %llu:%llu # Debit Card Approved # %llu", date, currentTime.getHours(), currentTime.getMinutes(),
         debitAccount.getId());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::client_exchangeCurrency(Number clientId, Currency_Type fromCurrency, Currency_Type toCurrency,
                                             Money amount) {
  currentTime += 10;

  double rate = 0.0;
  for (int i = 0; i < exchangeRateCount; i++) {
    if (exchangeRates[i].getSellingCurrency() == fromCurrency && exchangeRates[i].getBuyingCurrency() == toCurrency) {
      rate = exchangeRates[i].getExchangeRate();
      break;
    }
  }

  if (rate == 0.0) {
    throw std::invalid_argument("Exchange rate not found");
  }

  Money exchangedAmount = amount * rate;

  Account* fromBankAccount = nullptr;
  Account* toBankAccount = nullptr;

  for (int i = 0; i < bankAccountCount; i++) {
    Account& account = accounts[bankAccountsId[i].getAccountId()];
    if (account.getCurrency() == toCurrency) {
      toBankAccount = &account;
    }
    if (account.getCurrency() == fromCurrency) {
      fromBankAccount = &account;
    }
  }

  if (!fromBankAccount || !toBankAccount) {
    throw std::runtime_error("Bank accounts not found");
  }

  if (toBankAccount->getBalance() < exchangedAmount) {
    throw std::runtime_error("Can't perform exchange");
  }

  fromBankAccount->putMoney(amount);
  toBankAccount->takeMoney(exchangedAmount);

  printf("%llu # %llu:%llu # Currency Exchange Performed # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), exchangedAmount.getVal1(), exchangedAmount.getVal2());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::handleTelephoneInquiry(const std::string& name, Number accountId) {

  if (!isWithinBankingHours(currentTime)) {
    throw std::runtime_error("Bank is closed");
  }

  BankClient* client = nullptr;
  Number clientId = 0;
  for (int i = 0; i < clientCount; i++) {
    if (clients[i].getName() == name) {
      client = &clients[i];
      clientId = clients[i].getClientId();
      break;
    }
  }

  if (client == nullptr) {
    throw std::runtime_error("Client error. Only bank clients can make telephone inquiries");
  }

  bool found = false;
  Account* account = nullptr;

  for (int i = 0; i < clientCreditCount && !found; i++) {
    if (clientCreditAccounts[i].getAccountId() == accountId) {
      if (clientCreditAccounts[i].getClientId() == clientId) {
        account = &accounts[accountId];
        found = true;
      } else {
        throw std::runtime_error("Client error. Access denied");
      }
    }
  }

  for (int i = 0; i < clientDepositCount && !found; i++) {
    if (clientDepositAccounts[i].getAccountId() == accountId) {
      if (clientDepositAccounts[i].getClientId() == clientId) {
        account = &accounts[accountId];
        found = true;
      } else {
        throw std::runtime_error("Client error. Access denied");
      }
    }
  }

  if (!found) {
    throw std::runtime_error("Incorrect telephone operation. Unknown account");
  }

  currentTime += 5;

  printf("%llu # %llu:%llu # Telephone Balance of %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId, account->getBalance().getVal1(), account->getBalance().getVal2());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::handleOnlineCreditCardPayment(Number accountId, Currency_Type requestCurrency, Money amount) {

  Account* account = nullptr;
  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].getId() == accountId) {
      account = &accounts[i];
      break;
    }
  }
  if (!account) {
    throw std::runtime_error("Incorrect online operation. Unknown account");
  }

  if (account->getCurrency() != requestCurrency) {

    double rate = 0.0;
    for (int i = 0; i < exchangeRateCount; i++) {
      if (exchangeRates[i].getSellingCurrency() == account->getCurrency() &&
          exchangeRates[i].getBuyingCurrency() == requestCurrency) {
        rate = exchangeRates[i].getExchangeRate();
        break;
      }
    }
    if (rate == 0.0) {
      throw std::runtime_error("Incorrect online operation. Can't perform exchange");
    }
    amount = amount * rate;
  }

  if (account->getBalance() < amount) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  account->takeMoney(amount);

  printf("%llu # %llu:%llu # Online Credit Card Payment Approved %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId, account->getBalance().getVal1(), account->getBalance().getVal2());
}

void bankController::handleOnlineDebitCardPayment(Number accountId, Currency_Type requestCurrency, Money amount) {

  Account* account = nullptr;
  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].getId() == accountId) {
      account = &accounts[i];
      break;
    }
  }
  if (!account) {
    throw std::runtime_error("Incorrect online operation. Unknown account");
  }

  if (account->getCurrency() != requestCurrency) {

    double rate = 0.0;
    for (int i = 0; i < exchangeRateCount; i++) {
      if (exchangeRates[i].getSellingCurrency() == account->getCurrency() &&
          exchangeRates[i].getBuyingCurrency() == requestCurrency) {
        rate = exchangeRates[i].getExchangeRate();
        break;
      }
    }
    if (rate == 0.0) {
      throw std::runtime_error("Incorrect online operation. Can't perform exchange");
    }
    amount = amount * rate;
  }

  if (account->getBalance() < amount) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  account->takeMoney(amount);

  printf("%llu # %llu:%llu # Online Debit Card Payment Approved %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId, account->getBalance().getVal1(), account->getBalance().getVal2());
}

void bankController::handleOnlineTransferBetweenMyAccounts(Number fromAccountId, Number toAccountId, Money amount) {

  Account *fromAccount = nullptr, *toAccount = nullptr;
  Number clientId = 0;
  bool sameOwner = false;

  for (int i = 0; i < clientDepositCount; i++) {
    if (clientDepositAccounts[i].getAccountId() == fromAccountId) {
      clientId = clientDepositAccounts[i].getClientId();
      fromAccount = &accounts[fromAccountId];
      break;
    }
  }

  for (int i = 0; i < clientDepositCount; i++) {
    if (clientDepositAccounts[i].getAccountId() == toAccountId && clientDepositAccounts[i].getClientId() == clientId) {
      toAccount = &accounts[toAccountId];
      sameOwner = true;
      break;
    }
  }

  if (!fromAccount || !toAccount) {
    throw std::runtime_error("Incorrect online operation. Unknown account");
  }

  if (!sameOwner) {
    throw std::runtime_error("Incorrect online operation. Access denied");
  }

  bool isLoanAccount = false;
  Credit* credit = nullptr;
  for (int i = 0; i < clientCreditCount; i++) {
    if (clientCreditAccounts[i].getAccountId() == toAccountId) {
      isLoanAccount = true;
      credit = &credits[clientCreditAccounts[i].getCreditId()];
      break;
    }
  }

  Money transferAmount = amount;
  if (fromAccount->getCurrency() != toAccount->getCurrency()) {
    double rate = 0.0;
    for (int i = 0; i < exchangeRateCount; i++) {
      if (exchangeRates[i].getSellingCurrency() == fromAccount->getCurrency() &&
          exchangeRates[i].getBuyingCurrency() == toAccount->getCurrency()) {
        rate = exchangeRates[i].getExchangeRate();
        break;
      }
    }
    if (rate == 0.0) {
      throw std::runtime_error("Incorrect online operation. Can't perform exchange");
    }
    transferAmount = amount * rate;
  }

  if (fromAccount->getBalance() < amount) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  if (isLoanAccount && toAccount->getBalance() + transferAmount > credit->getReceived()) {
    throw std::runtime_error("Incorrect online operation. Invalid loan payment amount");
  }

  fromAccount->takeMoney(amount);
  toAccount->putMoney(transferAmount);

  printf("%llu # %llu:%llu # Online Transfer Between My Accounts Approved %llu -> %llu", date, currentTime.getHours(),
         currentTime.getMinutes(), fromAccountId, toAccountId);
}

void bankController::handleOnlineTransfer(Number fromAccountId, Number toAccountId, Money amount) {

  Account *fromAccount = nullptr, *toAccount = nullptr;
  Client_Type senderType = Client_Type::NOT_A_CLIENT;
  bool isVIP = false;

  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].getId() == fromAccountId) {
      fromAccount = &accounts[i];
    }
    if (accounts[i].getId() == toAccountId) {
      toAccount = &accounts[i];
    }
  }

  if (!fromAccount || !toAccount) {
    throw std::runtime_error("Incorrect online operation. Unknown account");
  }

  for (int i = 0; i < clientDepositCount; i++) {
    if (clientDepositAccounts[i].getAccountId() == fromAccountId) {
      for (int j = 0; j < clientCount; j++) {
        if (clients[j].getClientId() == clientDepositAccounts[i].getClientId()) {
          senderType = clients[j].getClientType();
          isVIP = (senderType == Client_Type::VIP_INDIVIDUAL_CLIENT || senderType == Client_Type::VIP_LEGAL_ENTITY);
          break;
        }
      }
      break;
    }
  }

  Money transferAmount = amount;
  Money fee(0, 0);

  if (!isVIP) {

    fee = amount * 0.01;
  }

  if (fromAccount->getCurrency() != toAccount->getCurrency()) {
    double rate = 0.0;
    for (int i = 0; i < exchangeRateCount; i++) {
      if (exchangeRates[i].getSellingCurrency() == fromAccount->getCurrency() &&
          exchangeRates[i].getBuyingCurrency() == toAccount->getCurrency()) {
        rate = exchangeRates[i].getExchangeRate();
        break;
      }
    }
    if (rate == 0.0) {
      throw std::runtime_error("Incorrect online operation. Can't perform exchange");
    }
    transferAmount = amount * rate;
  }

  if (fromAccount->getBalance() < (amount + fee)) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  fromAccount->takeMoney(amount + fee);
  toAccount->putMoney(transferAmount);

  printf("%llu # %llu:%llu # Online Transfer Approved %llu -> %llu", date, currentTime.getHours(),
         currentTime.getMinutes(), fromAccountId, toAccountId);
}

void bankController::handleOnlineInboundTransfer(Number accountId, Money amount) {

  Account* account = nullptr;
  for (int i = 0; i < accountCount; i++) {
    if (accounts[i].getId() == accountId) {
      account = &accounts[i];
      break;
    }
  }
  if (!account) {
    throw std::runtime_error("Incorrect online operation. Unknown account");
  }

  for (int i = 0; i < clientCreditCount; i++) {
    if (clientCreditAccounts[i].getAccountId() == accountId) {
      Credit& credit = credits[clientCreditAccounts[i].getCreditId()];
      if (account->getBalance() + amount > credit.getReceived()) {
        throw std::runtime_error("Incorrect online operation. Invalid loan payment amount");
      }
      break;
    }
  }

  account->putMoney(amount);

  printf("%llu # %llu:%llu # Online Inbound Transfer Approved %llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId);
}
