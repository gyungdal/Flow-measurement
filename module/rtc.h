#ifndef __RTC_H__
#define __RTC_H__

#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>
#include "type.h"

class RTC{
    public:
        RTC();
        void set(time_t);
        time_t* get();
        void setDayHandler(void (*dayHandler)());
        
    private:
        int pin;
        DS3231 clock;
};



#endif