#include "rtc.h"

#define AUTO_TIME

RTC::RTC(int pin){
    this->dayHandler = nullptr;
    this->secondHandler = nullptr;

    this->clock.begin();

    #ifdef AUTO_TIME
        this->clock.setDateTime(__DATE__, __TIME__);
    #endif
    
    this->clock.armAlarm1(false);
    this->clock.armAlarm2(false);
    this->clock.clearAlarm1();
    this->clock.clearAlarm2();
    // Set Alarm - Every second.
    // DS3231_EVERY_SECOND is available only on Alarm1.
    // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
    this->clock.setAlarm1(0, 0, 0, 0, DS3231_EVERY_SECOND);

    attachInterrupt(digitalPinToInterrupt(pin), alarmHandler, FALLING);
}

void RTC::alarmHandler(){
    if(this->secondHandler != nullptr){
        this->secondHandler();
    }
    
    RTCDateTime dt = clock.getDateTime();
    if(dt.hour == 0 && dt.minute == 0 && dt.second == 0){
        if(this->dayHandler != nullptr){
            this->dayHandler();
        }  
    }
}

void RTC::set(time_data_t time){
    // Manual (Year, Month, Day, Hour, Minute, Second)
    // 지정한 시간으로 초기화 (초는 0으로 고정)
    this->clock.setDateTime(time.year, time.month, time.day, time.hour, time.month, 0);
}

time_data_t RTC::get(){
    //RTC 모듈에서 데이터 가져옴
    RTCDateTime dt = clock.getDateTime();

    //데이터 처리 형식에 맞게 가공
    time_data_t result;
    result.year = dt.year;
    result.month = dt.month;
    result.day = dt.day;
    result.hour = dt.hour;
    result.minute = dt.minute;

    return result;
}