#ifndef __RTC_H__
#define __RTC_H__

#include <Wire.h>
#include <DS3231.h>
#include "type.h"

class RTC{
    public:
        RTC() = delete;
        RTC(int pin);
        void set(time_data_t);
        time_data_t get();
        void setDayHandler(void (*dayHandler)());
        
    private:
        DS3231 clock;
        void (*dayHandler())();
        void alarmHandler();
};



#endif