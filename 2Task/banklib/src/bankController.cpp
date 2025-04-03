#include "banklib/bankController.h"

#include <vector>

bankController::bankController(const std::map<uint64_t, Client> clients, const std::vector<BankAccount> bankAccountsId,
                               const std::map<uint64_t, Account> accounts, const std::map<uint64_t, Credit> credits,
                               const std::map<uint64_t, Deposit> deposits,
                               const std::vector<ExchangeRate> exchangeRates, const std::vector<Workplace> workplaces,
                               const ClientDepositAccount clientDepositAccounts,
                               const ClientCreditAccount clientCreditAccounts)
    : clients(std::move(clients)),
      bankAccountsId(std::move(bankAccountsId)),
      accounts(std::move(accounts)),
      credits(std::move(credits)),
      deposits(std::move(deposits)),
      exchangeRates(std::move(exchangeRates)),
      workplaces(std::move(workplaces)) {}

void bankController::startBankDay(Date day, Time time) {
  if (time != Time(8, 0)) {
    throw std::invalid_argument("Bank day can only start at 8:00");
  }
  currentTime = time;
  date = day;
  printf("%llu # %llu:%llu # Start of Bank Day", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankDay(Date day, Time time) {
  if (time != Time(19, 0)) {
    throw std::invalid_argument("Bank day can only end at 19:00");
  }
  currentTime = time;
  date = day;
  for (auto i = clients.begin(); i != clients.end(); i++) {
    Client& client = i->second;
    for (auto& j : client.getCredits()) {
      Credit& credit = credits[j];
      if (credit.getType() == Credit_Type::CHARGED_DAILY) {
        Account& account = accounts[credit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
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

    for (auto& j : client.getDeposits()) {
      Deposit& deposit = deposits[j];
      if (deposit.getType() == Deposit_Type::COMPOUNDED_DAILY_REMAINING ||
          deposit.getType() == Deposit_Type::COMPOUNDED_DAILY_MIN) {
        Account& account = accounts[deposit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (account.getBalance() * deposit.getDailyProcent()).getVal1(),
                    (account.getBalance() * deposit.getDailyProcent()).getVal2());
            try {
              bankAccount.takeMoney(account.getBalance() * deposit.getDailyProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(account.getBalance() * deposit.getDailyProcent());
          }
        }
      }
    }
  }

  printf("%llu # %llu:%llu # End of Bank Day", date, currentTime.getHours(), currentTime.getMinutes());
  int i = 0;
  for (auto& workplace : workplaces) {
    printf("%llu # %s # %llu # %llu\n", date, workspaceTypeToString(workplace.getWorkplaceType()).c_str(), i + 1,
           workplaceStats[i].dailyClients);
    workplaceStats[i++].dailyClients = 0;
  }
}

void bankController::endBankMonth(Date date, Time time) {
  this->currentTime = time;
  this->date = date;
  for (auto i = clients.begin(); i != clients.end(); i++) {
    Client& client = i->second;
    for (auto& j : client.getCredits()) {
      Credit& credit = credits[j];
      if (credit.getType() == Credit_Type::CHARGED_MONTHLY) {
        Account& account = accounts[credit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (credit.getReceived() * credit.getMonthlyProcent()).getVal1(),
                    (credit.getReceived() * credit.getMonthlyProcent()).getVal2());
            try {
              account.takeMoney(credit.getReceived() * credit.getMonthlyProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(credit.getReceived() * credit.getMonthlyProcent());
          }
        }
      }
    }

    for (auto& j : client.getDeposits()) {
      Deposit& deposit = deposits[j];
      if (deposit.getType() == Deposit_Type::COMPOUNDED_MONTHLY_REMAINING ||
          deposit.getType() == Deposit_Type::COMPOUNDED_MONTHLY_MIN) {
        Account& account = accounts[deposit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (account.getBalance() * deposit.getMonthlyProcent()).getVal1(),
                    (account.getBalance() * deposit.getMonthlyProcent()).getVal2());
            try {
              bankAccount.takeMoney(account.getBalance() * deposit.getMonthlyProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(account.getBalance() * deposit.getMonthlyProcent());
          }
        }
      }
    }
  }
  printf("%llu # %llu:%llu # End of Bank Month", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankQuarter(Date date, Time time) {
  this->currentTime = time;
  this->date = date;
  for (auto i = clients.begin(); i != clients.end(); i++) {
    Client& client = i->second;
    for (auto& j : client.getCredits()) {
      Credit& credit = credits[j];
      if (credit.getType() == Credit_Type::CHARGED_QUARTERLY) {
        Account& account = accounts[credit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (credit.getReceived() * credit.getQuarterlyProcent()).getVal1(),
                    (credit.getReceived() * credit.getQuarterlyProcent()).getVal2());
            try {
              account.takeMoney(credit.getReceived() * credit.getQuarterlyProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(credit.getReceived() * credit.getQuarterlyProcent());
          }
        }
      }
    }

    for (auto& j : client.getDeposits()) {
      Deposit& deposit = deposits[j];
      if (deposit.getType() == Deposit_Type::COMPOUNDED_QUARTERLY_REMAINING ||
          deposit.getType() == Deposit_Type::COMPOUNDED_QUARTERLY_MIN) {
        Account& account = accounts[deposit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (account.getBalance() * deposit.getQuarterlyProcent()).getVal1(),
                    (account.getBalance() * deposit.getQuarterlyProcent()).getVal2());
            try {
              bankAccount.takeMoney(account.getBalance() * deposit.getQuarterlyProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(account.getBalance() * deposit.getQuarterlyProcent());
          }
        }
      }
    }
  }

  printf("%llu # %llu:%llu # End of Bank Quarter", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankHalfYear(Date date, Time time) {
  this->currentTime = time;
  this->date = date;
  for (auto i = clients.begin(); i != clients.end(); i++) {
    Client& client = i->second;
    for (auto& j : client.getCredits()) {
      Credit& credit = credits[j];
      if (credit.getType() == Credit_Type::CHARGED_SEMIANNUALLY) {
        Account& account = accounts[credit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (credit.getReceived() * credit.getSemiannualProcent()).getVal1(),
                    (credit.getReceived() * credit.getSemiannualProcent()).getVal2());
            try {
              account.takeMoney(credit.getReceived() * credit.getSemiannualProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(credit.getReceived() * credit.getSemiannualProcent());
          }
        }
      }
    }

    for (auto& j : client.getDeposits()) {
      Deposit& deposit = deposits[j];
      if (deposit.getType() == Deposit_Type::COMPOUNDED_SEMIANUALLY_REMAINING ||
          deposit.getType() == Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN) {
        Account& account = accounts[deposit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (account.getBalance() * deposit.getSemiannuallyProcent()).getVal1(),
                    (account.getBalance() * deposit.getSemiannuallyProcent()).getVal2());
            try {
              bankAccount.takeMoney(account.getBalance() * deposit.getSemiannuallyProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(account.getBalance() * deposit.getSemiannuallyProcent());
          }
        }
      }
    }
  }
  printf("%llu # %llu:%llu # End of Bank Semi-Annual period", date, currentTime.getHours(), currentTime.getMinutes());
}

void bankController::endBankYear(Date date, Time time) {
  this->currentTime = time;
  this->date = date;
  for (auto i = clients.begin(); i != clients.end(); i++) {
    Client& client = i->second;
    for (auto& j : client.getCredits()) {
      Credit& credit = credits[j];
      if (credit.getType() == Credit_Type::CHARGED_ANNUALLY) {
        Account& account = accounts[credit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (credit.getReceived() * credit.getAnnualProcent()).getVal1(),
                    (credit.getReceived() * credit.getAnnualProcent()).getVal2());
            try {
              account.takeMoney(credit.getReceived() * credit.getAnnualProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(credit.getReceived() * credit.getAnnualProcent());
          }
        }
      }
    }

    for (auto& j : client.getDeposits()) {
      Deposit& deposit = deposits[j];
      if (deposit.getType() == Deposit_Type::COMPOUNDED_ANUALLY_REMAINING ||
          deposit.getType() == Deposit_Type::COMPOUNDED_ANUALLY_MIN) {
        Account& account = accounts[deposit.getAccountId()];
        for (auto& bankAcc : bankAccountsId) {
          if (accounts[bankAcc.getAccountId()].getCurrency() == account.getCurrency() && account.getBalance() > 0) {
            Account& bankAccount = accounts[bankAcc.getAccountId()];
            fprintf(stderr, "%llu # %llu:%llu # %llu -> %llu # %llu.%llu\n", date, currentTime.getHours(),
                    currentTime.getMinutes(), account.getId(), bankAccount.getId(),
                    (account.getBalance() * deposit.getAnnuallyProcent()).getVal1(),
                    (account.getBalance() * deposit.getAnnuallyProcent()).getVal2());
            try {
              bankAccount.takeMoney(account.getBalance() * deposit.getAnnuallyProcent());
            } catch (const std::invalid_argument& e) {
              fprintf(stderr, "Client Defaulted\n");
              exit(EXIT_FAILURE);
            }

            bankAccount.putMoney(account.getBalance() * deposit.getAnnuallyProcent());
          }
        }
      }
    }
  }
  printf("%llu # %llu:%llu # End of Bank Year", date, currentTime.getHours(), currentTime.getMinutes());
  int i = 0;
  for (auto& workplace : workplaces) {
    printf("%llu # %s # %llu # %llu\n", date, workspaceTypeToString(workplace.getWorkplaceType()).c_str(), i + 1,
           workplaceStats[i].yearlyClients);
    workplaceStats[i++].yearlyClients = 0;
  }
}

uint64_t bankController::registerNewClient(const std::string& name, Client_Type type) {
  if (isClientExists(name)) {
    throw std::invalid_argument("Client already exists");
  }
  currentTime += 15;
  Client client(clntID++, name, type);
  clients[client.getClientId()] = client;
  return client.getClientId();
}

bool bankController::isClientExists(const std::string& name) const {
  for (auto& client : clients) {
    if (client.second.getName() == name) {
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

void bankController::handlePersonalAppeal(const std::string& name, Client_Type type,
                                          std::vector<Operation_Type> operations) {
  Client client;
  bool isClient = false;
  for (auto& clientt : clients) {
    if (clientt.second.getName() == name) {
      client = clientt.second;
      isClient = true;
      break;
    }
  }

  if (!isClient) {
    bool canOpenAccount = false;
    for (auto& operation : operations) {
      if (operation == Operation_Type::OPEN_ACCOUNT || operation == Operation_Type::APPLY_DEBIT_CARD) {
        canOpenAccount = true;
        break;
      }
    }
    if (!canOpenAccount) {
      throw std::runtime_error("Client error. Wrong operation for new client");
    }
    client = clients[registerNewClient(name, type)];
  }
  for (auto& operation : operations) {
    if (!canOperationBeHandledAtWorkplace(operation, Workspace_Type::CLIENT_MANAGER, client.getClientType())) {
      throw std::runtime_error("Client error. Wrong operation for client");
    }
    handleClientOperation(client.getClientId(), operation);
  }
}

void bankController::handleClientOperation(uint64_t clientId, Operation_Type operation) {
  if (!isWithinBankingHours(currentTime)) {
    if (currentTime < Time(8, 0)) {
      throw std::runtime_error("Bank closed until 8:00");
    } else {
      throw std::runtime_error("Bank is closing at 19:00");
    }
  }

  Client client = clients[clientId];

  for (uint64_t i = 0; i < workplaces.size(); i++) {
    workplaceStats[i].dailyClients++;
    workplaceStats[i].yearlyClients++;
  }
}

void bankController::client_getBalance(uint64_t clientId, uint64_t accountId) {
  currentTime += 5;
  if (accounts.count(accountId) == 0) {
    throw std::invalid_argument("Client error. Unknown account");
  }

  Client& client = clients[clientId];
  if (client.getCredits().find(accountId) != client.getCredits().end()) {
    printf("%llu # %llu:%llu # Balance of %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
           accountId, accounts[accountId].getBalance().getVal1(), accounts[clientId].getBalance().getVal2());
    if (currentTime >= Time(19, 00))
      bankController::endBankDay(date, currentTime);
    return;
  }

  if (client.getDeposits().find(accountId) != client.getDeposits().end()) {
    printf("%llu # %llu:%llu # Balance of %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
           accountId, accounts[accountId].getBalance().getVal1(), accounts[accountId].getBalance().getVal2());
    if (currentTime >= Time(19, 00))
      bankController::endBankDay(date, currentTime);
    return;
  }
  throw std::invalid_argument("Client error. Access denied");
}

bool bankController::checkClientAccountCount(uint64_t clientId, Currency_Type currency, Client_Type clientType) {
  Client& client = clients[clientId];
  int count = 0;
  for (auto& id : client.getDeposits()) {
    Account& dep = accounts[id];
    if (dep.getCurrency() == currency)
      count++;
    if (count == StaticStorage::maxClientAccountsAmount[(uint64_t)currency][(uint64_t)clientType])
      return false;
  }
  return true;
}

void bankController::client_openAccount(uint64_t clientId, Money amount, Currency_Type currency,
                                        Client_Type clientType) {
  if (!checkClientAccountCount(clientId, currency, clientType))
    throw std::invalid_argument("Client error. Active account limit reached");

  currentTime += 10;
  Account account(accID++, Account_Type::DEPOSIT,
                  Money(-StaticStorage::openClientMoney[(int)currency][(int)clientType], 0), currency, clientId, depID);
  accounts[account.getId()] = account;
  Deposit deposit(depID++, 0.0, Deposit_Type::COMPOUNDED_ANUALLY_MIN, date, 0, accID - 1);
  deposits[deposit.getId()] = deposit;
  printf("%llu # %llu:%llu # Account Created %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
         account.getId(), account.getBalance().getVal1(), account.getBalance().getVal2());
}

void bankController::client_closeAccount(uint64_t clientId, uint64_t accountId) {
  currentTime += 25;

  if (accounts.find(accountId) != accounts.end()) {
    throw std::invalid_argument("Client error. Unknown account");
  }

  Client& client = clients[clientId];
  if (client.getCredits().find(accountId) == client.getCredits().end() &&
      client.getDeposits().find(accountId) == client.getDeposits().end()) {
    throw std::invalid_argument("Client error. Access denied");
  }

  Account account = accounts[accountId];

  if (account.getBalance() < Money(0, 0)) {
    throw std::invalid_argument("Client error. Negative balance on closure");
  }

  if (account.getType() == Account_Type::CREDIT) {
    Credit& credit = credits[account.getAssociatedId()];
    if (credit.getReceived() > account.getBalance()) {
      throw std::invalid_argument("Credit in action");
    }
    printf("%llu # %llu:%llu # Account closed %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
           accountId, account.getBalance().getVal1(), account.getBalance().getVal2());
    return;
  }

  Money fee = account.getBalance() * 0.015;
  if (fee < Money(10, 0)) {
    fee = Money(10, 0);
  }

  Money finalAmount = account.getBalance() - fee;
  printf("%llu # %llu:%llu # Account closed %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
         accountId, finalAmount.getVal1(), finalAmount.getVal2());

  if (currentTime >= Time(19, 00)) {
    bankController::endBankDay(date, currentTime);
  }
}

void bankController::client_withdrawFunds(uint64_t clientId, uint64_t accountId, Money amount) {
  currentTime += 10;

  if (accounts.find(accountId) == accounts.end()) {
    throw std::invalid_argument("Client error. Unknown account");
  }

  Client& client = clients[clientId];

  if ((client.getCredits().find(accountId) == client.getCredits().end() &&
       client.getDeposits().find(accountId) == client.getDeposits().end())) {
    throw std::invalid_argument("Client error. Access denied");
  }

  Account& account = accounts[accountId];

  Money fee(0, 0);

  if (account.getType() == Account_Type::DEPOSIT) {
    Deposit& deposit = deposits[account.getAssociatedId()];
    if (deposit.getDuration() * 30 <= date - deposit.getStartDate()) {
      fee = amount * 0.0025;
      if (fee < Money(2, 50)) {
        fee = Money(2, 50);
      }
    }
  }

  if (account.getBalance() > (amount + fee)) {
    Money totalDeposits(0, 0);
    for (auto& dep : accounts) {
      Account& depositAccount = dep.second;
      if ((depositAccount.getType() == Account_Type::DEPOSIT || depositAccount.getType() == Account_Type::DEBIT) &&
          depositAccount.getCurrency() == account.getCurrency()) {
        totalDeposits += depositAccount.getBalance();
      }
    }

    Money maxReserveUse = totalDeposits * StaticStorage::FRACTIONAL_RESERVE_LIMIT;
    if (maxReserveUse >= (amount + fee - account.getBalance())) {
      for (auto& dep : accounts) {
        Account& depositAccount = dep.second;
        if (depositAccount.getCurrency() == account.getCurrency()) {
          Money reserveAmount = depositAccount.getBalance() * StaticStorage::FRACTIONAL_RESERVE_LIMIT;
          depositAccount.takeMoney(reserveAmount);
          account.putMoney(reserveAmount);
        }
      }
    }
  }

  if (account.getBalance() < (amount + fee)) {
    throw std::invalid_argument("Client error. Insufficient funds");
  }

  account.takeMoney(amount + fee);

  printf("%llu # %llu:%llu # Funds Withdrawn %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
         accountId, account.getBalance().getVal1(), account.getBalance().getVal2());

  if (currentTime >= Time(19, 00)) {
    bankController::endBankDay(date, currentTime);
  }
}

void bankController::client_depositFunds(uint64_t clientId, uint64_t accountId, Money amount) {
  currentTime += 5;

  if (accounts.find(accountId) == accounts.end()) {
    throw std::invalid_argument("Client error. Unknown account");
  }
  Client& client = clients[clientId];
  Account& account = accounts[accountId];
  bool isLoanAccount = false;
  Credit& credit = credits[account.getAssociatedId()];

  if (account.getType() == Account_Type::CREDIT) {
    isLoanAccount = true;
  }

  if (isLoanAccount) {
    if (client.getCredits().find(accountId) == client.getCredits().end()) {
      throw std::invalid_argument("Client error. Access denied");
    }
  } else {
    if (client.getDeposits().find(accountId) == client.getDeposits().end()) {
      throw std::invalid_argument("Client error. Access denied");
    }
  }

  Money fee = amount * 0.0001;
  if (fee < Money(1, 0)) {
    fee = Money(1, 0);
  }

  if (isLoanAccount && (account.getBalance() + amount - fee) > credit.getReceived()) {
    throw std::invalid_argument("Client error. Invalid loan payment amount");
  }

  account.putMoney(amount - fee);

  printf("%llu # %llu:%llu # Fonds Credited %llu # %llu.%llu", date, currentTime.getHours(), currentTime.getMinutes(),
         accountId, account.getBalance().getVal1(), account.getBalance().getVal2());

  if (currentTime >= Time(19, 00)) {
    bankController::endBankDay(date, currentTime);
  }
}

void bankController::client_getLoan(uint64_t clientId, Currency_Type currency, Credit_Type creditType, Money amount) {
  currentTime += 45;

  if (clients.find(clientId) == clients.end()) {
    throw std::invalid_argument("Client not found");
  }
  Client& client = clients[clientId];

  if (amount.getValue() > StaticStorage::loanLimits[(int)currency][(int)client.getClientType()]) {
    throw std::invalid_argument("Client error. Loan amount exceeds limit");
  }

  bool hasSufficientFunds = false;
  for (auto& bankAcc : bankAccountsId) {
    Account& bankAccount = accounts[bankAcc.getAccountId()];
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

  client.getCredits().insert(accID);

  Account creditAccount(accID, Account_Type::CREDIT, Money(0, 0) - fee, currency, clientId, credID);
  accounts[accID++] = creditAccount;

  Credit credit(credID, StaticStorage::loanInterestRates[(int)currency][(int)client.getClientType()], creditType,
                amount, accID - 1);
  credits[credID++] = credit;

  printf("%llu # %llu:%llu # Loan Approved %llu", date, currentTime.getHours(), currentTime.getMinutes(),
         creditAccount.getId());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(19, 0));
  }
}

void bankController::client_requestCreditCard(uint64_t clientId, Currency_Type currency, Credit_Type creditType,
                                              Money amount) {
  currentTime += 10;

  if (clients.find(clientId) == clients.end()) {
    throw std::invalid_argument("Client not found");
  }
  Client& client = clients[clientId];

  if (amount.getValue() > StaticStorage::loanLimits[(int)currency][(int)client.getClientType()]) {
    throw std::invalid_argument("Client error. Credit amount exceeds limit");
  }

  CreditCardApplication application(clientId, currency, creditType, amount, date);
  pendingCreditCardApplications[clientId] = application;

  printf("%llu # %llu:%llu # Credit Card Application Received", date, currentTime.getHours(), currentTime.getMinutes());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(19, 0));
  }
}

void bankController::client_getCreditCard(uint64_t clientId, Currency_Type currency, Credit_Type creditType,
                                          Money amount) {
  currentTime += 5;

  if (clients.find(clientId) == clients.end()) {
    throw std::invalid_argument("Client not found");
  }
  Client& client = clients[clientId];

  auto appIt = pendingCreditCardApplications.find(clientId);
  if (appIt == pendingCreditCardApplications.end() || appIt->second.getCurrency() != currency ||
      appIt->second.getCreditType() != creditType || appIt->second.getAmount() != amount) {
    throw std::invalid_argument("Client error. No matching credit card application found");
  }

  uint64_t daysSinceApplication = date - appIt->second.getApplicationDate();
  if (daysSinceApplication < 3) {
    throw std::invalid_argument("Client error. Credit Card application not yet approved");
  }
  if (daysSinceApplication > 30) {
    throw std::invalid_argument("Client error. Credit Card application out of date");
  }

  bool hasSufficientFunds = false;
  for (auto& bankAcc : bankAccountsId) {
    Account& bankAccount = accounts[bankAcc.getAccountId()];
    if (bankAccount.getCurrency() == currency && bankAccount.getBalance() >= amount) {
      hasSufficientFunds = true;
      bankAccount.takeMoney(amount);
      break;
    }
  }

  if (!hasSufficientFunds) {
    throw std::runtime_error("Can't process credit card");
  }

  Money openingFee = Money(-StaticStorage::openClientMoney[(int)currency][(int)client.getClientType()], 0);
  client.getCredits().insert(accID);

  Account creditAccount(accID, Account_Type::CREDIT, amount + openingFee, currency, clientId, credID);
  accounts[accID++] = creditAccount;

  Credit credit(credID, StaticStorage::creditCardInterestRates[(int)currency][(int)client.getClientType()], creditType,
                amount, accID - 1);
  credits[credID++] = credit;

  printf("%llu # %llu:%llu # Credit Card Approved # %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), creditAccount.getId(), creditAccount.getBalance().getVal1(),
         creditAccount.getBalance().getVal2());

  pendingCreditCardApplications.erase(appIt);

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(19, 0));
  }
}

void bankController::client_makeDeposit(uint64_t clientId, Currency_Type currency, Deposit_Type depositType,
                                        uint64_t duration, Money amount) {
  currentTime += 20;

  if (clients.find(clientId) == clients.end()) {
    throw std::invalid_argument("Client not found");
  }
  Client& client = clients[clientId];

  uint64_t durationDays;
  int durationIdx;
  if (duration == 3) {
    durationDays = 90;
    durationIdx = 0;
  } else if (duration == 6) {
    durationDays = 182;
    durationIdx = 1;
  } else if (duration == 12) {
    durationDays = 365;
    durationIdx = 2;
  } else if (duration == 24) {
    durationDays = 730;
    durationIdx = 3;
  } else {
    throw std::invalid_argument("Invalid deposit duration");
  }

  double interestRate = StaticStorage::depositInterestRates[(int)currency][durationIdx][(int)client.getClientType()];
  if (interestRate == 0.0) {
    throw std::invalid_argument("Client error. Invalid deposit parameters");
  }

  Account account(accID, Account_Type::DEPOSIT, amount, currency, clientId, depID);
  accounts[accID++] = account;

  Deposit deposit(depID++, interestRate, depositType, date, durationDays, accID - 1);
  deposits[deposit.getId()] = deposit;

  client.getDeposits().insert(deposit.getId());

  for (auto& bankAcc : bankAccountsId) {
    Account& bankAccount = accounts[bankAcc.getAccountId()];
    if (bankAccount.getCurrency() == currency && bankAccount.getBalance() < Money(0, 0)) {
      Money maxReserve = account.getBalance() * 0.8;
      account.takeMoney(maxReserve);
      bankAccount.putMoney(maxReserve);
    }
  }

  printf("%llu # %llu:%llu # Deposit Approved # %llu", date, currentTime.getHours(), currentTime.getMinutes(),
         account.getId());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(19, 0));
  }
}

void bankController::client_requestDebitCard(uint64_t clientId, Currency_Type currency, Deposit_Type depositType) {
  currentTime += 10;

  if (clients.find(clientId) == clients.end()) {
    throw std::invalid_argument("Client not found");
  }
  Client& client = clients[clientId];

  bool isMinimumType =
      (depositType == Deposit_Type::COMPOUNDED_DAILY_MIN || depositType == Deposit_Type::COMPOUNDED_MONTHLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_QUARTERLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN || depositType == Deposit_Type::COMPOUNDED_ANUALLY_MIN);

  printf("%llu # %llu:%llu # Debit Card Application Received", date, currentTime.getHours(), currentTime.getMinutes());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(20, 0));
  }
}

void bankController::client_getDebitCard(uint64_t clientId, Currency_Type currency, Deposit_Type depositType) {
  currentTime += 5;

  if (clients.find(clientId) == clients.end()) {
    throw std::invalid_argument("Client not found");
  }
  Client& client = clients[clientId];

  auto appIt = pendingDebitCardApplications.find(clientId);
  if (appIt == pendingDebitCardApplications.end() || appIt->second.getCurrency() != currency ||
      appIt->second.getDepositType() != depositType) {
    throw std::invalid_argument("Client error. No matching credit card application found");
  }

  uint64_t daysSinceApplication = date - appIt->second.getApplicationDate();
  if (daysSinceApplication < 5) {
    throw std::invalid_argument("Client error. Debit Card application not yet approved");
  }
  if (daysSinceApplication > 15) {
    throw std::invalid_argument("Client error. Debit Card application out of date");
  }

  Money openingFee = Money(-StaticStorage::openClientMoney[(int)currency][(int)client.getClientType()], 0);

  Account debitAccount(accID, Account_Type::DEPOSIT, openingFee, currency, clientId, depID);
  accounts[accID++];

  bool isMinimumType =
      (depositType == Deposit_Type::COMPOUNDED_DAILY_MIN || depositType == Deposit_Type::COMPOUNDED_MONTHLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_QUARTERLY_MIN ||
       depositType == Deposit_Type::COMPOUNDED_SEMIANUALLY_MIN || depositType == Deposit_Type::COMPOUNDED_ANUALLY_MIN);

  Deposit deposit(
      depID, StaticStorage::debitCardInterestRates[(int)currency][isMinimumType ? 1 : 0][(int)client.getClientType()],
      depositType, date, 0, accID - 1);
  deposits[depID++] = deposit;

  printf("%llu # %llu:%llu # Debit Card Approved # %llu", date, currentTime.getHours(), currentTime.getMinutes(),
         debitAccount.getId());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(19, 0));
  }
}

void bankController::client_exchangeCurrency(uint64_t clientId, Currency_Type fromCurrency, Currency_Type toCurrency,
                                             Money amount) {
  currentTime += 10;

  double rate = 0.0;
  for (auto& rates : exchangeRates) {
    if (rates.getSellingCurrency() == fromCurrency && rates.getBuyingCurrency() == toCurrency) {
      rate = rates.getExchangeRate();
      break;
    }
  }

  if (rate == 0.0) {
    throw std::invalid_argument("Exchange rate not found");
  }

  Money exchangedAmount = amount * rate;

  Account* fromBankAccount = nullptr;
  Account* toBankAccount = nullptr;

  for (int i = 0; i < bankAccountsId.size(); i++) {
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
    endBankDay(date, Time(19, 0));
  }
}

void bankController::handleTelephoneInquiry(const std::string& name, uint64_t accountId) {

  if (!isWithinBankingHours(currentTime)) {
    throw std::runtime_error("Bank is closed");
  }

  uint64_t clientId = -1;
  for (int i = 0; i < clients.size(); i++) {
    if (clients[i].getName() == name) {
      clientId = clients[i].getClientId();
      break;
    }
  }
  if (clientId == -1) {
    throw std::runtime_error("Client error. Only bank clients can make telephone inquiries");
  }

  Client& client = clients[clientId];

  bool found = false;

  if (accounts.find(accountId) == accounts.end()) {
    throw std::invalid_argument("Incorrect telephone operation. Unknown account");
  }

  if (client.getCredits().find(accountId) == client.getCredits().end() ||
      client.getDeposits().find(accountId) == client.getDeposits().end()) {
    throw std::runtime_error("Client error. Access denied");
  }

  Account& account = accounts[accountId];
  currentTime += 5;

  printf("%llu # %llu:%llu # Telephone Balance of %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId, account.getBalance().getVal1(), account.getBalance().getVal2());

  if (currentTime >= Time(19, 00)) {
    endBankDay(date, Time(19, 0));
  }
}

void bankController::handleOnlineCreditCardPayment(uint64_t accountId, Currency_Type requestCurrency, Money amount) {
  if (accounts.find(accountId) == accounts.end()) {
    throw std::invalid_argument("Incorrect online operation. Unknown account");
  }
  Account& account = accounts[accountId];

  if (account.getCurrency() != requestCurrency) {

    double rate = 0.0;
    for (int i = 0; i < exchangeRates.size(); i++) {
      if (exchangeRates[i].getSellingCurrency() == account.getCurrency() &&
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

  if (account.getBalance() < amount) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  account.takeMoney(amount);

  printf("%llu # %llu:%llu # Online Credit Card Payment Approved %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId, account.getBalance().getVal1(), account.getBalance().getVal2());
}

void bankController::handleOnlineDebitCardPayment(uint64_t accountId, Currency_Type requestCurrency, Money amount) {

  if (accounts.find(accountId) == accounts.end()) {
    throw std::invalid_argument("Incorrect online operation. Unknown account");
  }
  Account& account = accounts[accountId];

  if (account.getCurrency() != requestCurrency) {

    double rate = 0.0;
    for (int i = 0; i < exchangeRates.size(); i++) {
      if (exchangeRates[i].getSellingCurrency() == account.getCurrency() &&
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

  if (account.getBalance() < amount) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  account.takeMoney(amount);

  printf("%llu # %llu:%llu # Online Debit Card Payment Approved %llu # %llu.%llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId, account.getBalance().getVal1(), account.getBalance().getVal2());
}

void bankController::handleOnlineTransferBetweenMyAccounts(uint64_t fromAccountId, uint64_t toAccountId, Money amount) {
  if (accounts.find(fromAccountId) == accounts.end() || accounts.find(toAccountId) == accounts.end()) {
    throw std::runtime_error("Incorrect online operation. Unknown account");
  }

  Account &fromAccount = accounts[fromAccountId], toAccount = accounts[toAccountId];

  if (fromAccount.getClientId() != toAccount.getClientId()) {
    throw std::runtime_error("Incorrect online operation. Access denied");
  }

  uint64_t clientId = fromAccount.getClientId();
  Client& client = clients[clientId];

  Credit* credit = nullptr;
  bool isLoanAccount = false;
  if (client.getCredits().find(toAccountId) != client.getCredits().end()) {
    isLoanAccount = true;
    credit = &credits[toAccount.getAssociatedId()];
  }

  Money transferAmount = amount;
  if (fromAccount.getCurrency() != toAccount.getCurrency()) {
    double rate = 0.0;
    for (int i = 0; i < exchangeRates.size(); i++) {
      if (exchangeRates[i].getSellingCurrency() == fromAccount.getCurrency() &&
          exchangeRates[i].getBuyingCurrency() == toAccount.getCurrency()) {
        rate = exchangeRates[i].getExchangeRate();
        break;
      }
    }
    if (rate == 0.0) {
      throw std::runtime_error("Incorrect online operation. Can't perform exchange");
    }
    transferAmount = amount * rate;
  }

  if (fromAccount.getBalance() < amount) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  if (isLoanAccount && toAccount.getBalance() + transferAmount > credit->getReceived()) {
    throw std::runtime_error("Incorrect online operation. Invalid loan payment amount");
  }

  fromAccount.takeMoney(amount);
  toAccount.putMoney(transferAmount);

  printf("%llu # %llu:%llu # Online Transfer Between My Accounts Approved %llu -> %llu", date, currentTime.getHours(),
         currentTime.getMinutes(), fromAccountId, toAccountId);
}

void bankController::handleOnlineTransfer(uint64_t fromAccountId, uint64_t toAccountId, Money amount) {
  Client_Type senderType = Client_Type::NOT_A_CLIENT;
  bool isVIP = false;

  if (accounts.find(fromAccountId) == accounts.end() || accounts.find(toAccountId) == accounts.end()) {
    throw std::runtime_error("Incorrect online operation. Unknown account");
  }

  Account &fromAccount = accounts[fromAccountId], toAccount = accounts[toAccountId];

  if (fromAccount.getClientId() != toAccount.getClientId()) {
    throw std::runtime_error("Incorrect online operation. Access denied");
  }

  senderType = clients[fromAccount.getClientId()].getClientType();
  isVIP = (senderType == Client_Type::VIP_INDIVIDUAL_CLIENT || senderType == Client_Type::VIP_LEGAL_ENTITY);

  Money transferAmount = amount;
  Money fee(0, 0);

  if (!isVIP) {

    fee = amount * 0.01;
  }

  if (fromAccount.getCurrency() != toAccount.getCurrency()) {
    double rate = 0.0;
    for (int i = 0; i < exchangeRates.size(); i++) {
      if (exchangeRates[i].getSellingCurrency() == fromAccount.getCurrency() &&
          exchangeRates[i].getBuyingCurrency() == toAccount.getCurrency()) {
        rate = exchangeRates[i].getExchangeRate();
        break;
      }
    }
    if (rate == 0.0) {
      throw std::runtime_error("Incorrect online operation. Can't perform exchange");
    }
    transferAmount = amount * rate;
  }

  if (fromAccount.getBalance() < (amount + fee)) {
    throw std::runtime_error("Incorrect online operation. Insufficient funds");
  }

  fromAccount.takeMoney(amount + fee);
  toAccount.putMoney(transferAmount);

  printf("%llu # %llu:%llu # Online Transfer Approved %llu -> %llu", date, currentTime.getHours(),
         currentTime.getMinutes(), fromAccountId, toAccountId);
}

void bankController::handleOnlineInboundTransfer(uint64_t accountId, Money amount) {

  if (accounts.find(accountId) == accounts.end()) {
    throw std::invalid_argument("Incorrect online operation. Unknown account");
  }
  Account& account = accounts[accountId];

  Client& client = clients[account.getClientId()];

  if (client.getCredits().find(accountId) != client.getCredits().end()) {
    Credit& credit = credits[account.getAssociatedId()];
    if (account.getBalance() + amount > credit.getReceived()) {
      throw std::runtime_error("Incorrect online operation. Invalid loan payment amount");
    }
  }
  account.putMoney(amount);

  printf("%llu # %llu:%llu # Online Inbound Transfer Approved %llu", date, currentTime.getHours(),
         currentTime.getMinutes(), accountId);
}
