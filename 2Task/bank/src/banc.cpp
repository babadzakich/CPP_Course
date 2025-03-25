#include <banklib/bankController.h>

int main() {
  uint64_t accountCount;
  scanf("Accounts %llu\n", &accountCount);
  Account accounts[accountCount];
  for (int i = 0; i < accountCount; i++) {
    uint64_t id;
    std::string type;
    uint64_t val1, val2;
    std::string currency;
    scanf("%llu # %99[a-zA-Z0-9/_ ] # %llu.%llu # %99[a-zA-Z0-9/_ ]\n", &id, &type, &val1, &val2, &currency);
    accounts[i] = Account(id, stringToAccountType(type), Money(val1, val2), stringToCurrency(currency));
  }

  uint64_t debitsCount;
  scanf("Debits %llu\n", &debitsCount);
  Deposit debits[debitsCount];
  for (int i = 0; i < debitsCount; i++) {
    uint64_t id;
    double procent;
    std::string type;
    Date startDate;
    uint64_t duration;
    scanf("%llu # %le # %99[a-zA-Z0-9/_ ] # %llu # %llu\n", &id, &procent, &type, &startDate, &duration);
    debits[i] = Deposit(id, procent, stringToDepositType(type), startDate, duration);
  }

  uint64_t creditsCount;
  scanf("Credits %llu\n", &creditsCount);
  Credit credits[creditsCount];
  for (int i = 0; i < creditsCount; i++) {
    uint64_t id;
    double procent;
    std::string type;
    uint64_t receivedVal1, receivedVal2;
    scanf("%llu # %le # %llu.%llu # %99[a-zA-Z0-9/_ ]\n", &id, &procent, &receivedVal1, &receivedVal2, &type);
    credits[i] = Credit(id, procent, stringToCreditType(type), Money(receivedVal1, receivedVal2));
  }

  uint64_t clientsCount;
  scanf("Clients %llu\n", &clientsCount);
  Client clients[clientsCount];
  for (int i = 0; i < clientsCount; i++) {
    uint64_t clientId;
    std::string name;
    std::string clientType;
    scanf("%llu # %99[a-zA-Z0-9/_ ] # %99[a-zA-Z0-9/_ ]\n", &clientId, &name, &clientType);
    clients[i] = Client(clientId, name, stringToClientType(clientType));
  }

  uint64_t clientDepositCount;
  scanf("Client Debit %llu\n", &clientDepositCount);
  ClientDepositAccount clientDeposits[clientDepositCount];
  for (int i = 0; i < clientDepositCount; i++) {
    uint64_t clientId, accountId, depositId;
    scanf("%llu # %llu # %llu\n", &clientId, &accountId, &depositId);
    clientDeposits[i] = ClientDepositAccount(clientId, accountId, depositId);
  }

  uint64_t bankAccountsCount;
  scanf("Bank Accounts %llu\n", &bankAccountsCount);
  BankAccount bankAccounts[bankAccountsCount];
  for (int i = 0; i < bankAccountsCount; i++) {
    uint64_t id;
    scanf("%llu\n", &id);
    bankAccounts[i] = BankAccount(id);
  }

  uint64_t clientCreditCount;
  scanf("Client Credit %llu\n", &clientCreditCount);
  ClientCreditAccount clientCredits[clientCreditCount];
  for (int i = 0; i < clientCreditCount; i++) {
    uint64_t clientId, accountId, creditId;
    scanf("%llu # %llu # %llu\n", &clientId, &accountId, &creditId);
    clientCredits[i] = ClientCreditAccount(clientId, accountId, creditId);
  }

  uint64_t workplaceCount;
  scanf("Workplaces %llu\n", &workplaceCount);
  Workplace workplaces[workplaceCount];
  for (int i = 0; i < workplaceCount; i++) {
    std::string type;
    uint64_t id;
    scanf("%99[a-zA-Z0-9/_ ] # %llu\n", &type, &id);
    workplaces[i] = Workplace(stringToWorkspaceType(type), id);
  }

  uint64_t exchangeRatesCount;
  scanf("Exchange Rates %llu\n", &exchangeRatesCount);
  ExchangeRate exchangeRates[exchangeRatesCount];
  for (int i = 0; i < exchangeRatesCount; i++) {
    std::string from, to;
    double rate;
    scanf("%99[a-zA-Z0-9/_ ] # %99[a-zA-Z0-9/_ ] # %le\n", &from, &to, &rate);
    exchangeRates[i] = ExchangeRate(stringToCurrency(from), stringToCurrency(to), rate);
  }

  bankController controller(clients, clientsCount, bankAccounts, bankAccountsCount, accounts, accountCount, credits,
                            creditsCount, debits, debitsCount, exchangeRates, exchangeRatesCount, workplaces,
                            workplaceCount, clientDeposits, clientDepositCount, clientCredits, clientCreditCount);
  controller.~bankController();
  return 0;
}