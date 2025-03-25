#ifndef CREDIT_H
#define CREDIT_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Credit_Type.h"

class Credit {
 private:
  uint64_t id;
  double procent;
  Credit_Type type;
  Money received;
  uint64_t accountId;

 public:
  Credit() : id(0), procent(0.0), type(Credit_Type::CHARGED_DAILY), received(Money(0, 0)), accountId(0) {}
  Credit(uint64_t id, double procent, Credit_Type type, Money received, uint64_t accountId);

  uint64_t getId() const;
  double getProcent() const;
  Credit_Type getType() const;
  Money getReceived() const;
  uint64_t getAccountId() const;

  double getDailyProcent() const;
  double getMonthlyProcent() const;
  double getQuarterlyProcent() const;
  double getSemiannualProcent() const;
  double getAnnualProcent() const;
};

#endif  // CREDIT_H
