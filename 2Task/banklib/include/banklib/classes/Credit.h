#ifndef CREDIT_H
#define CREDIT_H

#include "banklib/Money.h"
#include "banklib/Types.h"
#include "banklib/enums/Credit_Type.h"

class Credit {
 private:
  Number id;
  double procent;
  Credit_Type type;
  Money received;

 public:
  Credit() : id(0), procent(0.0), type(Credit_Type::CHARGED_DAILY), received(Money(0, 0)) {}
  Credit(Number id, double procent, Credit_Type type, Money received);

  Number getId() const;
  double getProcent() const;
  Credit_Type getType() const;
  Money getReceived() const { return received; }

  double getDailyProcent() const;
  double getMonthlyProcent() const;
  double getQuarterlyProcent() const;
  double getSemiannualProcent() const;
  double getAnnualProcent() const;
};

#endif  // CREDIT_H
