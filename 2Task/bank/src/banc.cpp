#include "banklib/bankController.h"

int main() {
  Amount accountCount;
  scanf("Accounts %llu\n", &accountCount);
  std::vector<Account> accounts;
  accounts.reserve(accountCount);  // Добавить остальным
  for (int i = 0; i < accountCount; i++) {
    Id id;
    char type[100];  // string на char, и сделать string = char
    unsigned long long val1, val2;
    char currency[100];
    Id clientId, associatedId;
    scanf("%llu # %99[a-zA-Z0-9/_ ] # %llu.%llu # %99[a-zA-Z0-9/_ ] # %llu # %llu\n", &id, &type, &val1, &val2, &currency, &clientId, &associatedId);
    accounts.push_back(Account(id, stringToAccountType(type), Money(val1, val2), stringToCurrency(currency), clientId, associatedId));
  }

  Amount debitsCount;
  scanf("Debits %llu\n", &debitsCount);
  std::vector<Deposit> debits;
  debits.reserve(debitsCount);
  for (int i = 0; i < debitsCount; i++) {
    Id id;
    double procent;
    char type[100];
    Date startDate;
    Amount duration;
    Id accountId;
    scanf("%llu # %le # %99[a-zA-Z0-9/_ ] # %llu # %llu # %llu\n", &id, &procent, &type, &startDate, &duration, &accountId);
    debits.push_back(Deposit(id, procent, stringToDepositType(type), startDate, duration, accountId));
  }

  Amount creditsCount;
  scanf("Credits %llu\n", &creditsCount);
  std::vector<Credit> credits;
  credits.reserve(creditsCount);
  for (int i = 0; i < creditsCount; i++) {
    Id id;
    double procent;
    char type[100];
    unsigned long long receivedVal1, receivedVal2;
    Id clientId;
    scanf("%llu # %le # %llu.%llu # %99[a-zA-Z0-9/_ ] # %llu\n", &id, &procent, &receivedVal1, &receivedVal2, &type, &clientId);
    credits.push_back(Credit(id, procent, stringToCreditType(type), Money(receivedVal1, receivedVal2), clientId));
  }

  Amount clientsCount;
  scanf("Clients %llu\n", &clientsCount);
  std::vector<Client> clients;
  clients.reserve(clientsCount);
  for (int i = 0; i < clientsCount; i++) {
    Id clientId;
    char name[100];
    char clientType[100];
    scanf("%llu # %99[a-zA-Z0-9/_ ] # %99[a-zA-Z0-9/_ ]\n", &clientId, &name, &clientType);
    clients.push_back(Client(clientId, name, stringToClientType(clientType)));
  }

  Amount bankAccountsCount;
  scanf("Bank Accounts %llu\n", &bankAccountsCount);
  std::vector<BankAccount> bankAccounts;
  bankAccounts.reserve(bankAccountsCount);
  for (int i = 0; i < bankAccountsCount; i++) {
    Id id;
    scanf("%llu\n", &id);
    bankAccounts.push_back(BankAccount(id));
  }

  Amount workplaceCount;
  scanf("Workplaces %llu\n", &workplaceCount);
  std::vector<Workplace> workplaces;
  workplaces.reserve(workplaceCount);
  for (int i = 0; i < workplaceCount; i++) {
    char type[100];
    Id id;
    scanf("%99[a-zA-Z0-9/_ ] # %llu\n", &type, &id);
    workplaces.push_back(Workplace(stringToWorkspaceType(type), id));
  }

  Amount exchangeRatesCount;
  scanf("Exchange Rates %llu\n", &exchangeRatesCount);
  std::vector<ExchangeRate> exchangeRates;
  exchangeRates.reserve(exchangeRatesCount);
  for (int i = 0; i < exchangeRatesCount; i++) {
    char from[100], to[100];
    double rate;
    scanf("%99[a-zA-Z0-9/_ ] # %99[a-zA-Z0-9/_ ] # %le\n", &from, &to, &rate);
    exchangeRates.push_back(ExchangeRate(stringToCurrency(from), stringToCurrency(to), rate));
  }

  bankController controller(clients, bankAccounts, accounts, credits,
                            debits, exchangeRates, workplaces);
  controller.~bankController();
  return 0;
}