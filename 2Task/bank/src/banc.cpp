#include <banklib/bankController.h>

int main() {
  Number accountCount;
  scanf("Accounts %llu\n", &accountCount);
  Account accounts[accountCount];
  for (int i = 0; i < accountCount; i++) {
    Number id;
    std::string type;
    Number val1, val2;
    std::string currency;
    scanf("%llu # %99[a-zA-Z0-9/_ ] # %llu.%llu # %99[a-zA-Z0-9/_ ]\n", &id, &type, &val1, &val2, &currency);
    accounts[i] = Account(id, stringToAccountType(type), Money(val1, val2), stringToCurrency(currency));
  }

  Number debitsCount;
  scanf("Debits %llu\n", &debitsCount);
  Deposit debits[debitsCount];
  for (int i = 0; i < debitsCount; i++) {
    Number id;
    double procent;
    std::string type;
    Date startDate;
    Number duration;
    scanf("%llu # %le # %99[a-zA-Z0-9/_ ] # %llu # %llu\n", &id, &procent, &type, &startDate, &duration);
    debits[i] = Deposit(id, procent, stringToDepositType(type), startDate, duration);
  }

  Number creditsCount;
  scanf("Credits %llu\n", &creditsCount);
  Credit credits[creditsCount];
  for (int i = 0; i < creditsCount; i++) {
    Number id;
    double procent;
    std::string type;
    Number receivedVal1, receivedVal2;
    scanf("%llu # %le # %llu.%llu # %99[a-zA-Z0-9/_ ]\n", &id, &procent, &receivedVal1, &receivedVal2, &type);
    credits[i] = Credit(id, procent, stringToCreditType(type), Money(receivedVal1, receivedVal2));
  }

  Number clientsCount;
  scanf("Clients %llu\n", &clientsCount);
  BankClient clients[clientsCount];
  for (int i = 0; i < clientsCount; i++) {
    Number clientId;
    std::string name;
    std::string clientType;
    scanf("%llu # %99[a-zA-Z0-9/_ ] # %99[a-zA-Z0-9/_ ]\n", &clientId, &name, &clientType);
    clients[i] = BankClient(clientId, name, stringToClientType(clientType));
  }

  Number clientDepositCount;
  scanf("Client Debit %llu\n", &clientDepositCount);
  ClientDepositAccount clientDeposits[clientDepositCount];
  for (int i = 0; i < clientDepositCount; i++) {
    Number clientId, accountId, depositId;
    scanf("%llu # %llu # %llu\n", &clientId, &accountId, &depositId);
    clientDeposits[i] = ClientDepositAccount(clientId, accountId, depositId);
  }

  Number bankAccountsCount;
  scanf("Bank Accounts %llu\n", &bankAccountsCount);
  BankAccount bankAccounts[bankAccountsCount];
  for (int i = 0; i < bankAccountsCount; i++) {
    Number id;
    scanf("%llu\n", &id);
    bankAccounts[i] = BankAccount(id);
  }

  Number clientCreditCount;
  scanf("Client Credit %llu\n", &clientCreditCount);
  ClientCreditAccount clientCredits[clientCreditCount];
  for (int i = 0; i < clientCreditCount; i++) {
    Number clientId, accountId, creditId;
    scanf("%llu # %llu # %llu\n", &clientId, &accountId, &creditId);
    clientCredits[i] = ClientCreditAccount(clientId, accountId, creditId);
  }

  Number workplaceCount;
  scanf("Workplaces %llu\n", &workplaceCount);
  Workplace workplaces[workplaceCount];
  for (int i = 0; i < workplaceCount; i++) {
    std::string type;
    Number id;
    scanf("%99[a-zA-Z0-9/_ ] # %llu\n", &type, &id);
    workplaces[i] = Workplace(stringToWorkspaceType(type), id);
  }

  Number exchangeRatesCount;
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