#include "rtc.h"

#define AUTO_TIME

RTC::RTC(int pin){
    this->dayHandler = nullptr;

    this->clock.begin();

    //활성화시 컴파일된 시간으로 RTC 모듈 초기화
    #ifdef AUTO_TIME
        this->clock.setDateTime(__DATE__, __TIME__);
    #endif
    
    this->clock.armAlarm1(false);
    this->clock.armAlarm2(false);
    this->clock.clearAlarm1();
    this->clock.clearAlarm2();

    //매일 0시 0분 1초에 인터럽트 발생
    clock.setAlarm1(0, 0, 0, 1, DS3231_MATCH_H_M_S);

    attachInterrupt(digitalPinToInterrupt(pin), alarmHandler, FALLING);
}

void RTC::alarmHandler(){
    if(this->dayHandler != nullptr){
        this->dayHandler();
    }  
}

void RTC::setDayHandler(void (*dayHandler)()){
    this->dayHandler = dayHandler;
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