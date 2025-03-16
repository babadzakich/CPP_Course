//
// Created by babadzakich on 14.03.2025.
//

#ifndef CREDIT_H
#define CREDIT_H

#include "BankClasses.h"


class Credit {
private:
    Number id;
    double procent;
    Credit_Type type;
    Date startDate;
    Number duration;
public:
    Credit (Number id, double procent, Credit_Type type, Date startDate, Number duration);
    Number getId();
    double getDailyProcent();
    double getMonthlyProcent();
    double getQuarterlyProcent();
    double getSemiannualProcent();
    double getAnnualProcent();
    Credit_Type getType();
    Date getStartDate();
    Number getDuration();
};



#endif //CREDIT_H
