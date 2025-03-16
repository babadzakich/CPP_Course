//
// Created by babadzakich on 14.03.2025.
//

#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "BankClasses.h"

class Deposit {
private:
    Number id;
    double procent;
    Deposit_Type type;
    Date startDate;
    Number duration;
public:
    Number getId() const;
    double getDailyProcent() const;
    double getMonthlyProcent() const;
    double getQuarterlyProcent() const;
    double getSemiannuallyProcent() const;
    double getAnnuallyProcent() const;
    Deposit_Type getType() const;
    Date getStartDate() const;
    Number getDuration() const;
    Deposit(Number id, double procent, Deposit_Type type, Date startDate, Number duration);
};



#endif //DEPOSIT_H
