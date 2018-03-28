#include <Arduino.h>
/*
#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
double liter;

//https://github.com/jarzebski/Arduino-DS3231/
//Mega Interrupt : 2, 3, 18, 19, 20, 21

void setup(){
    Serial.begin(115200);
    
    clock.begin();

    clock.armAlarm1(false);
    clock.armAlarm2(false);
    clock.clearAlarm1();
    clock.clearAlarm2();
    // Set Alarm - Every 00h:00m:00s in each day
    // setAlarm1(Date or Day, Hour, Minute, Second, Mode, Armed = true)
    clock.setAlarm1(0, 0, 0, 0, DS3231_MATCH_H_M_S);
}

void loop(){
    //One Day
    if(clock.isAlarm2()){

        liter = 0;        
    }
}
*/
#include "test.h"

Test test;

void setup(){
    Serial.begin(115200);
}

void loop(){
    test.eeprom();    
    delay(1000);

}
