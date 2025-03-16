//
// Created by babadzakich on 14.03.2025.
//

#ifndef BANKCONTROLLER_H
#define BANKCONTROLLER_H

#include "enums/Bank_enums.h"
#include "classes/BankClasses.h"

#include <vector>
#include <string>
#include <utility>

using Number = unsigned long long;
using Money = double;
// using Time = std::pair<int, int>;

class Time {
    private:
        int hours;
        int minutes;
    public:

        Time(int hours, int minutes) : hours(hours), minutes(minutes) {}

        Time& operator++ () {
            minutes++;
            hours += minutes / 60;
            minutes %= 60;
            hours %= 24;
            return *this;
        }

        Time& operator++ (int) {
            Time temp = *this;
            ++*this;
            return temp;
        }

        bool operator < (const Time& other) const {
            return hours < other.hours || (hours == other.hours && minutes < other.minutes);
        }

        bool operator > (const Time& other) const {
            return hours > other.hours || (hours == other.hours && minutes > other.minutes);
        }

        bool operator == (const Time& other) const {
            return hours == other.hours && minutes == other.minutes;
        }

        bool operator != (const Time& other) const {
            return !(*this == other);
        }

        Number getHours() const {
            return hours;
        }

        Number getMinutes() const {
            return minutes;
        }
};
using Date = Number;


class bankController {
    private:
        Time currentTime;
        Date date;

        BankClient clients[100000];
        Number clientCount;

        BankAccount accounts[100000];
        Number accountCount;

        Account clientAccounts[100000];
        Number clientAccountCount;

        Credit credits[100000];
        Number creditCount;
        
        Deposit deposits[100000];
        Number depositCount;

        ExchangeRate exchangeRates[100000];
        Number exchangeRateCount;

        Workplace workplaces[100000];
        Number workplaceCount;

        ClientAccount clientAccounts[100000];
        Number clientAccountCount;

        ClientDebt clientDebts[100000];
        Number clientDebtCount;
    public:
        bankController();
        void startBankDay(Date day, Time time);
        void endBankDay();
        void endBankMonth();
        void endBankQuarter();
        void endBankHalfYear();
        void endBankYear();
};



#endif //BANKCONTROLLER_H
