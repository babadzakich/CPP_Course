//
// Created by babadzakich on 14.03.2025.
//

#include "Credit.h"

Credit::Credit(Number id, double procent, Credit_Type type, Date startDate, Number duration) 
    : id(id), procent(procent), type(type), startDate(startDate), duration(duration) {}

Number Credit::getId() { return id; }

double Credit::getDailyProcent() { return procent / 365; }

double Credit::getMonthlyProcent() { return procent / 12; }

double Credit::getQuarterlyProcent() { return procent / 4; }

double Credit::getSemiannualProcent() { return procent / 2; }

double Credit::getAnnualProcent() { return procent; }

Credit_Type Credit::getType() { return type; }

Date Credit::getStartDate() { return startDate; }

Number Credit::getDuration() { return duration; }
