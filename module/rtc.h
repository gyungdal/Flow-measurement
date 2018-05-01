#ifndef __RTC_H__
#define __RTC_H__

#include <Wire.h>
#include <DS3231.h>
#include "type.h"

class RTC{
    public:
        RTC();
        void set(time_t);
        time_t* get();
    private:
        int pin;
        DS3231 clock;
};



#endif