#include <gtest/gtest.h>

#include "banklib/bankController.h"

class BankTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Initialize test data
    std::map<uint64_t, Client> clients;
    std::vector<BankAccount> bankAccounts;
    std::map<uint64_t, Account> accounts;
    std::map<uint64_t, Credit> credits;
    std::map<uint64_t, Deposit> deposits;
    std::vector<ExchangeRate> rates;
    std::vector<Workplace> workplaces;
    ClientDepositAccount clientDepositAccounts;
    ClientCreditAccount clientCreditAccounts;

    // Setup basic exchange rates
    rates.push_back(ExchangeRate(Currency_Type::RUB, Currency_Type::USD, 0.011));
    rates.push_back(ExchangeRate(Currency_Type::USD, Currency_Type::RUB, 91.0));

    // Setup workplaces
    workplaces.push_back(Workplace(Workspace_Type::CASH_DESK, 2));
    workplaces.push_back(Workplace(Workspace_Type::CLIENT_MANAGER, 2));

    controller = new bankController(clients, bankAccounts, accounts, credits, deposits, rates, workplaces,
                                    clientDepositAccounts, clientCreditAccounts);
  }

  void TearDown() override { delete controller; }

  bankController* controller;
};

TEST_F(BankTest, RegisterNewClient) {
  EXPECT_NO_THROW({
    uint64_t clientId = controller->registerNewClient("John Doe", Client_Type::INDIVIDUAL_CLIENT);
    EXPECT_GT(clientId, 0);
  });
}

TEST_F(BankTest, DuplicateClientRegistration) {
  controller->registerNewClient("John Doe", Client_Type::INDIVIDUAL_CLIENT);
  EXPECT_THROW(controller->registerNewClient("John Doe", Client_Type::INDIVIDUAL_CLIENT), std::invalid_argument);
}

TEST_F(BankTest, BankDayOperations) {
  EXPECT_NO_THROW({
    controller->startBankDay(1, Time(8, 0));
    controller->endBankDay(1, Time(19, 0));
  });
}

TEST_F(BankTest, InvalidBankDayTime) {
  EXPECT_THROW(controller->startBankDay(1, Time(9, 0)), std::invalid_argument);

  EXPECT_THROW(controller->endBankDay(1, Time(18, 0)), std::invalid_argument);
}

TEST_F(BankTest, ClientAccountOperations) {
  uint64_t clientId = controller->registerNewClient("Jane Doe", Client_Type::INDIVIDUAL_CLIENT);

  EXPECT_NO_THROW({
    controller->startBankDay(1, Time(8, 0));
    controller->client_openAccount(clientId, Money(1000, 0), Currency_Type::RUB, Client_Type::INDIVIDUAL_CLIENT);
  });
}

TEST_F(BankTest, CurrencyExchange) {
  uint64_t clientId = controller->registerNewClient("Bob Smith", Client_Type::INDIVIDUAL_CLIENT);

  EXPECT_NO_THROW({
    controller->startBankDay(1, Time(8, 0));
    controller->client_exchangeCurrency(clientId, Currency_Type::RUB, Currency_Type::USD, Money(1000, 0));
  });
}

TEST_F(BankTest, InvalidCurrencyExchange) {
  uint64_t clientId = controller->registerNewClient("Alice Brown", Client_Type::INDIVIDUAL_CLIENT);

  EXPECT_THROW(controller->client_exchangeCurrency(clientId, Currency_Type::RUB, Currency_Type::EUR, Money(1000, 0)),
               std::invalid_argument);
}

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
