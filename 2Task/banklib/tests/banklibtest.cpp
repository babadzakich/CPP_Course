#include <gtest/gtest.h>
#include <sstream>
#include "banklib/bankController.h"

class BankTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // Initialize test data
    Client clients[1] = {Client(1, "Test Client", Client_Type::INDIVIDUAL_CLIENT)};
    BankAccount bankAccounts[1] = {BankAccount(1)};
    Account accounts[1] = {Account(1, Account_Type::DEBIT, Money(1000, 0), Currency_Type::RUB)};
    Credit credits[1] = {Credit(1, 10.0, Credit_Type::CHARGED_MONTHLY, Money(1000, 0))};
    Deposit deposits[1] = {Deposit(1, 5.0, Deposit_Type::COMPOUNDED_MONTHLY_REMAINING, 20230101, 365)};
    ExchangeRate rates[1] = {ExchangeRate(Currency_Type::RUB, Currency_Type::USD, 0.011)};
    Workplace workplaces[1] = {Workplace(Workspace_Type::CLIENT_MANAGER, 1)};
    ClientDepositAccount clientDeposits[1] = {ClientDepositAccount(1, 1, 1)};
    ClientCreditAccount clientCredits[1] = {ClientCreditAccount(1, 1, 1)};

    bank = new bankController(clients, 1, bankAccounts, 1, accounts, 1, credits, 1, deposits, 1, rates, 1, workplaces,
                              1, clientDeposits, 1, clientCredits, 1);
    testing::internal::CaptureStdout(); // Start capturing stdout at setup
  }

  void TearDown() override {
    captured_output = testing::internal::GetCapturedStdout(); // Store final output
    delete bank;
  }

  bankController* bank;
  std::string captured_output;
};

// Test bank day start
TEST_F(BankTest, StartBankDay) {
  bank->startBankDay(20230101, Time(8, 0));
  std::string output = testing::internal::GetCapturedStdout();
  testing::internal::CaptureStdout(); // Restart capture for next test
  EXPECT_EQ(output, "20230101 # 8:0 # Start of Bank Day");
}

// Test invalid bank day start time
TEST_F(BankTest, InvalidStartBankDay) {
  EXPECT_THROW(bank->startBankDay(20230101, Time(9, 0)), std::invalid_argument);
}

// Test bank day end
TEST_F(BankTest, EndBankDay) {
  bank->startBankDay(20230101, Time(8, 0));
  bank->endBankDay(20230101, Time(19, 0));  
  std::string output = testing::internal::GetCapturedStdout();
  testing::internal::CaptureStdout();
  EXPECT_TRUE(output.find("End of Bank Day") != std::string::npos);
}

// Test invalid bank day end time
TEST_F(BankTest, InvalidEndBankDay) {
  bank->startBankDay(20230101, Time(8, 0));
  EXPECT_THROW(bank->endBankDay(20230101, Time(20, 0)), std::invalid_argument);  
}

// Test client operation during banking hours
TEST_F(BankTest, HandleClientOperationDuringBankingHours) {
  bank->startBankDay(20230101, Time(8, 0));
  Operation_Type ops[] = {Operation_Type::CHECK_BALANCE};
  EXPECT_NO_THROW(bank->handleClientOperation("Test Client", ops, 1));
}

// Test client operation outside banking hours
TEST_F(BankTest, HandleClientOperationOutsideBankingHours) {
  bank->startBankDay(20230101, Time(8, 0));
  bank->endBankDay(20230101, Time(19, 0)); 
  Operation_Type ops[] = {Operation_Type::CHECK_BALANCE};
  EXPECT_THROW(bank->handleClientOperation("Test Client", ops, 1), std::runtime_error);
}

// Test personal appeal for new client
TEST_F(BankTest, HandlePersonalAppealNewClient) {
  bank->startBankDay(20230101, Time(8, 0));
  EXPECT_NO_THROW(
      bank->handlePersonalAppeal("New Client", Client_Type::INDIVIDUAL_CLIENT, Operation_Type::OPEN_ACCOUNT, 1));
}

// Test invalid personal appeal for new client
TEST_F(BankTest, HandleInvalidPersonalAppealNewClient) {
  bank->startBankDay(20230101, Time(8, 0));
  EXPECT_THROW(
      bank->handlePersonalAppeal("New Client", Client_Type::INDIVIDUAL_CLIENT, Operation_Type::CHECK_BALANCE, 1),
      std::runtime_error);
}

// Test monthly operations
TEST_F(BankTest, MonthlyOperations) {
  bank->startBankDay(20230101, Time(8, 0));
  bank->endBankMonth(20230131, Time(20, 0));
  std::string output = testing::internal::GetCapturedStdout();
  testing::internal::CaptureStdout(); // Restart capture for next test
  EXPECT_TRUE(output.find("End of Bank Month") != std::string::npos);
}

// Test quarterly operations
TEST_F(BankTest, QuarterlyOperations) {
  bank->startBankDay(20230101, Time(8, 0));
  bank->endBankQuarter(20230331, Time(20, 0));
  std::string output = testing::internal::GetCapturedStdout();
  testing::internal::CaptureStdout(); // Restart capture for next test
  EXPECT_TRUE(output.find("End of Bank Quarter") != std::string::npos);
}

// Add more test cases here...

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
