//
// Created by babadzakich on 14.03.2025.
//

#include "bankController.h"
 
void bankController::startBankDay(Date day, Time time) {
    if (time != Time(8, 0)) {
        throw std::invalid_argument("Bank day can only start at 8:00");
    }
    currentTime = time;
    date = day;
    printf("%llu # %llu:%llu # Start of Bank Day", date, currentTime.getHours(), currentTime.getMinutes());
}

