#include <U8glib.h>
#include <TimerOne.h>
#include <Wire.h>
#include <DS3231.h>

#include "module/korean.h"
#include "module/type.h"
#include "module/storage.cpp"
#include "module/sensor.cpp"
#include "module/motor.cpp"

#define DEBUG 1

/*
    U8GLIB_ST7920_128X64_1X(sck, mosi, cs [, reset])
    Serial mode (PSB = LOW)
    sck: Pin with label "E"
    mosi: Pin with label "RW"
    cs: Pin with label "RS".
    Example: U8GLIB_ST7920_128X64_1X(sck, mosi, cs [, reset]) is equivalent to U8GLIB_ST7920_128X64_1X(E, RW, RS, RST) for many display modules. 
*/

//한글은 한 글자당 12, Space는 4

//u8glib는 글자를 그릴때에는 끝점부터잡고, xbm은 시작점을 잡는듯

//SW SPI-> HW SPI로 변경
U8GLIB_ST7920_128X64_1X u8g(49);
//font 한개 사이즈 : 11

const int RTC_PIN = 2;

DS3231 clock;

Storage storage;
Sensor sensor;
Motor motor;

user_t user;

static button_t buttons[] = {
    { .type = SCALE, .pin = 22, .lastState = LOW},
    { .type = SAVE, .pin = 23, .lastState = LOW},
    { .type = ZERO, .pin = 24, .lastState = LOW},
    { .type = MODE, .pin = 25, .lastState = LOW},
    { .type = UP, .pin = 26, .lastState = LOW},
    { .type = DOWN, .pin = 27, .lastState = LOW}
};


void timeSet(time_t time){
    // Manual (Year, Month, Day, Hour, Minute, Second)
    // 지정한 시간으로 초기화 (초는 0으로 고정)
    clock.setDateTime(time.year, time.month, time.day, time.hour, time.month, 0);
}

time_t* timeGet(){
    //RTC 모듈에서 데이터 가져옴
    RTCDateTime dt = clock.getDateTime();

    //데이터 처리 형식에 맞게 가공
    time_t* result = new time_t;
    result->year = dt.year;
    result->month = dt.month;
    result->day = dt.day;
    result->hour = dt.hour;
    result->minute = dt.minute;
    result->second = dt.second;
    return result;
}
static inline void dayHandler(){
    //EEPROM 에 저장된 리스트 Load
    eeprom_list_t* list = storage.get();

    //저장된 리스트의 개수가 0인지 확인
    if(list->length != 0){
        eeprom_item_t* buffer = new eeprom_item_t[list->length + 1];
        memcpy(buffer, list->items, sizeof(eeprom_item_t) * list->length);
        delete[] list->items;
        list->items = buffer;
    }else{
        list->items = new eeprom_item_t;
    }
    
    list->items[list->length].index = list->length;
    list->items[list->length].amount = user.sensor.getAmount();
    
    //센서 정보 초기화
    user.sensor.resetAmount();

    #ifdef NOT_USE_RTC     
    list->items[list->length].time.year = 2018;
    list->items[list->length].time.month = 5;
    list->items[list->length].time.day = 2;
    list->items[list->length].time.hour = 20;
    list->items[list->length].time.minute = 30;
    #else
    time_t* time = timeGet();
    list->items[list->length].time.year = time->year;
    list->items[list->length].time.month = time->month;
    list->items[list->length].time.day = time->day;
    list->items[list->length].time.hour = time->hour;
    list->items[list->length].time.minute = time->minute;
    delete time;
    #endif
    list->length += 1;
    storage.set(list);

    delete[] list->items;
    delete list;
}

static liquid_amount_t lastAmount;

static inline void secondTimer(){
    //모터에서 몇초간 신호가 오지 않았는지 계산
    uint64_t diff;
    uint64_t micro = micros();

    if(user.motor.lastTime > micro){
        diff = UINT64_MAX - user.motor.lastTime;
        diff += micro;
    }else{
        diff = micro - user.motor.lastTime;
    }

    //1초 이상 차이
    if(diff > 1000000){
        user.motor.isError = true;
    }

    //1초당 시간당 유속량 계산
    liquid_amount_t nowAmount = user.sensor.getAmount();
    double liter = (nowAmount.liter - lastAmount.liter);
    liter += ((double)(nowAmount.milliLiter - lastAmount.milliLiter) / 1000.0);
    user.sensor.waterPerHour = (uint32_t)(liter * 3600);
    lastAmount = nowAmount;
}

static inline void motorFirstPinInterrupt(){
    motor.firstTick();
}

static inline void motorSecondPinInterrupt(){
    motor.secondTick();
}

static inline void sensorFirstPinInterrupt(){
    sensor.tickHandler();
}

void setup() {
    Serial.begin(115200);
    #ifdef DEBUG
        Serial.println("[START]");
    #endif

    user.sensor.sensorType = 0;
    user.sensor.pin = 3;
    
    //EEPROM 정리
    #ifdef EEPROM_CLEAR
        storage.clear();
    #endif
    
    user.motor.type = NOT_RUN;
    user.motor.pwmPin = A0;
    user.motor.sigPin[0] = 18;
    user.motor.sigPin[1] = 19;

    sensor.begin(&user.sensor);
    #ifdef DEBUG
      Serial.println("[BEGIN] Sensor");
    #endif
    motor.begin(&user.motor);
    #ifdef DEBUG
      Serial.println("[BEGIN] Motor");
    #endif

    clock.begin();
    //활성화시 컴파일된 시간으로 RTC 모듈 초기화
    #ifdef AUTO_TIME
        clock.setDateTime(__DATE__, __TIME__);
    #endif
    
    clock.armAlarm1(false);
    clock.armAlarm2(false);
    clock.clearAlarm1();
    clock.clearAlarm2();

    //매일 0시 0분 1초에 인터럽트 발생
    clock.setAlarm1(0, 0, 0, 1, DS3231_MATCH_H_M_S);

    pinMode(RTC_PIN, INPUT_PULLUP);
    pinMode(user.motor.sigPin[0], INPUT_PULLUP);
    pinMode(user.motor.sigPin[1], INPUT_PULLUP);
    pinMode(user.sensor.pin, INPUT_PULLUP);
    
    //RTC에서 오는 1일 마다 발생하는 타이머 설정 
    attachInterrupt(digitalPinToInterrupt(RTC_PIN), 
            dayHandler, RISING);
    attachInterrupt(digitalPinToInterrupt(user.motor.sigPin[0]), 
            motorFirstPinInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(user.motor.sigPin[1]), 
            motorSecondPinInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(user.sensor.pin), 
            sensorFirstPinInterrupt, RISING);
            
    //1초 타이머 설정
    //Timer1.initialize(1000000);
    //Timer1.attachInterrupt(secondTimer);


    //현재 페이지 설정 (MAIN_VIEW)
    user.lastPage = MAIN_VIEW;
    user.nowPage = MAIN_VIEW;
    for(uint8_t i = 0;i<sizeof(buttons) / sizeof(button_t);i++){
        pinMode(buttons[i].pin, INPUT);
    }

    eeprom_setting_t* saveData = storage.readSetting();
    if(saveData != nullptr){
        #ifdef DEBUG 
            Serial.println("Load Lastdata");
            Serial.print("injectionPerHour : ");
            Serial.println(saveData->injectionPerHour);
            Serial.print("type : ");
            Serial.println(saveData->type);
            Serial.print("sensor Type : ");
            Serial.println(saveData->sensorType);
        #endif
        user.motor.injectionPerHour = saveData->injectionPerHour;
        user.motor.type = saveData->type;
        user.sensor.sensorType = saveData->sensorType;
        delete saveData;
    }
    update();
    #ifdef DEBUG
        Serial.println("[DEBUG] DRAW SETUP");
    #endif
}

//메인
static inline void mainViewDraw(){
    char* str = (char*)calloc(sizeof(char), 100);
    switch(user.motor.type){
        case RUN_BY_INJECTION_PER_HOUR : {
            sprintf(str, " : %ucc", user.motor.injectionPerHour);
            u8g.drawStr(PER_HOUR_XBM.width, 18, str);
            u8g.drawXBM(0, 6, PER_HOUR_XBM.width, PER_HOUR_XBM.height, PER_HOUR_XBM.value);
            break;
        }
        case RUN_BY_SCALE : {
            sprintf(str, " : 1/%d", motor_scale_list[user.motor.scale]);
            u8g.drawStr(WATER_SCALE_XBM.width, 18, str);
            u8g.drawXBM(0, 6, WATER_SCALE_XBM.width, WATER_SCALE_XBM.height, WATER_SCALE_XBM.value);
            break;
        }
        case NOT_RUN : {
            u8g.drawXBM(0, 6, MEASURE_ONLY_WATER_XBM.width, MEASURE_ONLY_WATER_XBM.height, MEASURE_ONLY_WATER_XBM.value);
            break;
        }
    }
    u8g.drawXBM(0, 19, WATER_SPEED_XBM.width, WATER_SPEED_XBM.height, WATER_SPEED_XBM.value);
    u8g.drawXBM(0, 32, WATER_COUNT_XBM.width, WATER_COUNT_XBM.height, WATER_COUNT_XBM.value);
    u8g.drawXBM(0, 45, SENSOR_XBM.width, SENSOR_XBM.height, SENSOR_XBM.value);

    memset(str, 0x00, 100);
    sprintf(str, " : %dL/h", user.sensor.waterPerHour);
    u8g.drawStr(24, 31, str);
    memset(str, 0x00, 100);
    sprintf(str, " : %dL", user.sensor.liter);
    u8g.drawStr(36, 44, str);
    memset(str, 0x00, 100);
    sprintf(str, " : %u", user.sensor.sensorType);
    u8g.drawStr(24, 57, str);   
    
    delete[] str;
}

//메뉴 
static inline void menuViewDraw(){
    xbm_t* list = new xbm_t[4];
    list[0] = INJECTION_PER_HOUR_XBM;
    list[1] = MEASURE_ONLY_WATER_XBM;
    list[2] = AMOUNT_BY_DAY_XBM;
    list[3] = SET_CURRENT_TIME_XBM;

    for(uint8_t i = 0;i < user.itemLength + 1;i++){
        u8g.setDefaultForegroundColor();
        if(user.nowIndex == i){
            u8g.drawBox(0, 6 + (13 * i), 128, 13);
            u8g.setDefaultBackgroundColor();
        }
        u8g.drawXBM(0, 6 + (13 * i), list[i].width, list[i].height, list[i].value);
    }


    delete[] list;
}

//시간당 비율 설정 뷰
static inline void setScaleViewDraw(){
    char* str = (char*)calloc(sizeof(char), 100);
    u8g.drawXBM(23, 19, DRUG_WATER_SCALE_XBM.width, DRUG_WATER_SCALE_XBM.height, DRUG_WATER_SCALE_XBM.value);
    sprintf(str, "%u", motor_scale_list[user.motor.scale]);
    u8g.drawStr(64 - (u8g.getStrWidth(str) / 2), 46, str);
    delete[] str;
}

//물약 장전 여부
static inline void SelectLoadingDrugViewDraw(){
    u8g.drawXBM(36, 12, LOAD_DRUG_XBM.width, LOAD_DRUG_XBM.height, LOAD_DRUG_XBM.value);
    u8g.drawStr(86, 25, "?");
    u8g.drawXBM(46, 25, YES_XBM.width, YES_XBM.height, YES_XBM.value);
    u8g.drawXBM(34, 41, NO_XBM.width, NO_XBM.height, NO_XBM.value);
}

//물약 장전 중지
static inline void stopLoadingDrugViewDraw(){
    u8g.drawXBM(32, 19, LOAD_DRUG_XBM.width, LOAD_DRUG_XBM.height, LOAD_DRUG_XBM.value);
    u8g.drawXBM(84, 19, ING_XBM.width, ING_XBM.height, ING_XBM.value);
    u8g.drawXBM(40, 46, STOP_XBM.width, STOP_XBM.height, STOP_XBM.value);
}

//~~모드로 동작
static inline void runningViewDraw(){
    xbm_t* list = new xbm_t[3];
    list[0] = INJECTION_PER_HOUR_XBM;
    list[1] = DRUG_WATER_SCALE_XBM;
    list[2] = MEASURE_ONLY_WATER_XBM;
    u8g.drawXBM(0, 19, list[user.mode].width, list[user.mode].height, list[user.mode].value);
    u8g.drawXBM(0, 32, RUNNING_IN_MODE_XBM.width, RUNNING_IN_MODE_XBM.height, RUNNING_IN_MODE_XBM.value);
    user.nowPage = MAIN_VIEW;
    user.mode = NOTHING_MODE;
    delete[] list;
    delay(300);
    update();
}

//시간당 주입량 조절
static inline void injectionPerHourViewDraw(){
    char* str = (char*)calloc(sizeof(char), 100);
    u8g.drawXBM(32, 19, INJECTION_PER_HOUR_XBM.width, INJECTION_PER_HOUR_XBM.height, INJECTION_PER_HOUR_XBM.value);
    sprintf(str, "%u", user.motor.injectionPerHour);
    u8g.drawStr(64 - (u8g.getStrWidth(str) / 2), 46, str);
    delete[] str;
}

//센서 선택
static inline void selectSensorViewDraw(){
    
    char* str = (char*)calloc(sizeof(char), 100);
    u8g.drawXBM(23, 19, SELECT_SENSOR_TYPE_XBM.width, SELECT_SENSOR_TYPE_XBM.height, SELECT_SENSOR_TYPE_XBM.value);
    sprintf(str, "%u", user.sensor.sensorType);
    u8g.drawStr(64 - (u8g.getStrWidth(str) / 2), 46, str);
    delete[] str;
}

static inline void setCurrentTimeViewDraw(){   
    Serial.println("Draw Current Time");

    #ifdef NOT_USE_RTC
    u8g.drawXBM(23, 12, SET_CURRENT_TIME_XBM.width, SET_CURRENT_TIME_XBM.height, SET_CURRENT_TIME_XBM.value);
    u8g.drawStr(64 - (u8g.getStrWidth("2018/05/02 13:49") / 2), 38, "2018/05/02 13:49");
    u8g.drawXBM(30, 38, SAVE_AFTER_SETTING_XBM.width, SAVE_AFTER_SETTING_XBM.height, SAVE_AFTER_SETTING_XBM.value);

    #else

    char* str = (char*)calloc(sizeof(char), 100);
    u8g.drawXBM(23, 12, SET_CURRENT_TIME_XBM.width, SET_CURRENT_TIME_XBM.height, SET_CURRENT_TIME_XBM.value);
    sprintf(str, "%u/%u/%u %u:%u", user.time.time.year, user.time.time.month, user.time.time.day, 
                        user.time.time.hour, user.time.time.minute);
    Serial.println(str);
    u8g.drawStr(64 - (u8g.getStrWidth(str) / 2), 38, str);
    const char* text[] = {
        "Year",
        "Month",
        "Date",
        "Hour",
        "Minute"
    };
    switch(user.time.index){
        case TIME_YEAR : 
        case TIME_MONTH : 
        case TIME_DAY :
        case TIME_HOUR : 
        case TIME_MINUTE : 
            sprintf(str, "Set %s", text[user.time.index]);
            break;
        case TIME_DONE : {
            u8g.drawXBM(30, 38, SAVE_AFTER_SETTING_XBM.width, SAVE_AFTER_SETTING_XBM.height, SAVE_AFTER_SETTING_XBM.value);
            break;
        }
    }
    if(user.time.index != TIME_DONE){
        u8g.drawStr(64 - (u8g.getStrWidth(str) / 2), 51, str);
    }
    delete[] str;

    #endif
}

static inline void logViewDraw(){
    u8g.drawXBM(32, 19, LOAD_DRUG_XBM.width, LOAD_DRUG_XBM.height, LOAD_DRUG_XBM.value);
    u8g.drawXBM(84, 19, ING_XBM.width, ING_XBM.height, ING_XBM.value);
    u8g.drawXBM(40, 46, STOP_XBM.width, STOP_XBM.height, STOP_XBM.value);
}

void update(){
    u8g.firstPage();
    u8g.setFont(u8g_font_7x14);
    while(u8g.nextPage()){
        switch(user.nowPage){
            case MAIN_VIEW : {
                mainViewDraw();
                break;
            }
            case MODE_VIEW : {
                menuViewDraw();
                break;
            }case SET_SCALE_VIEW : {
                setScaleViewDraw();
                break;
            }
            case INJECTION_PER_HOUR_VIEW : {
                injectionPerHourViewDraw();
                break;
            }
            case LOG_VIEW : {
                logViewDraw();
                break;
            }
            case SET_CURRENT_TIME_VIEW : {
                setCurrentTimeViewDraw();
                break;
            }
            case SELECT_LOAD_DRUG_VIEW : {
                SelectLoadingDrugViewDraw();
                break;
            }
            case LOADING_DRUG_VIEW : {
                stopLoadingDrugViewDraw();
                break;
            }
            case SELECT_SENSOR_VIEW : {
                selectSensorViewDraw();
                break;
            }
            case CLEAR_COUNT_VIEW: {
                u8g.drawXBM(0, 0, CLEAR_COUNT_XBM.width, CLEAR_COUNT_XBM.height, CLEAR_COUNT_XBM.value);
                u8g.drawXBM(0, 24, YES_XBM.width, YES_XBM.height, YES_XBM.value);
                u8g.drawXBM(0, 48, NO_XBM.width, NO_XBM.height, NO_XBM.value);
                break;
            }
            case RUNNING_IN_MODE_VIEW : {
                runningViewDraw();
                break;
            }
            default:{
                break;
            }
        }
    }
}

void loop() {  
    eeprom_setting_t checker;
    checker.sensorType = user.sensor.sensorType;
    checker.type = user.motor.type;
    checker.injectionPerHour = user.motor.injectionPerHour;
    display_menu_t beforePage = user.nowPage;
    for(uint8_t i = 0;i<sizeof(buttons) / sizeof(button_t);i++){
        int state = digitalRead(buttons[i].pin);
        if(state != buttons[i].lastState){
            if(buttons[i].lastState != LOW){
                uint64_t diffTime = millis() - buttons[i].lastTime;
                if(diffTime < 120)
                    continue;
                #ifdef DEBUG
                    Serial.print("[PUSH] ");
                    Serial.print(buttons[i].pin);
                    Serial.print(" : ");
                    Serial.print((unsigned long)diffTime);
                    Serial.println("ms");
                #endif
                switch(buttons[i].type){
                    case UP :{
                        switch(user.nowPage){
                            case SET_CURRENT_TIME_VIEW : {
                                switch(user.time.index){
                                    case TIME_YEAR : {
                                        user.time.time.year++;
                                        break;
                                    }
                                    case TIME_MONTH : {
                                        user.time.time.month += (user.time.time.month < 12 ? 1 : 0);
                                        break;
                                    }
                                    case TIME_DAY : {
                                        const uint8_t dayLimitTable[] = {
                                            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
                                        };
                                        uint8_t dayLimit = dayLimitTable[user.time.time.month - 1];
                                        if(user.time.time.month == 2){
                                            if((user.time.time.year % 4 == 0 && 
                                                    user.time.time.year % 100 != 0) 
                                                        || user.time.time.year % 400 == 0){
                                                dayLimit = 29;
                                            }
                                        }
                                        user.time.time.day += (user.time.time.day < dayLimit ? 1 : 0);
                                        break;
                                    }
                                    case TIME_HOUR : {
                                        user.time.time.hour += (user.time.time.hour < 23 ? 1 : 0);
                                        break;
                                    }
                                    case TIME_MINUTE : {
                                        user.time.time.minute += (user.time.time.minute < 59 ? 1 : 0);
                                        break;
                                    }
                                }
                                break;
                            }
                            case SELECT_LOAD_DRUG_VIEW : {
                                user.nowPage = LOADING_DRUG_VIEW;
                                break;
                            }
                            case LOADING_DRUG_VIEW : {
                                user.nowPage = RUNNING_IN_MODE_VIEW;
                                break;
                            }
                            case SELECT_SENSOR_VIEW : {
                                if(user.sensor.sensorType < 6)
                                    user.sensor.sensorType++; 
                                break;
                            }
                            case SET_SCALE_VIEW : {
                                user.motor.scale += (user.motor.scale < 73 ? 1 : 0);
                                break;
                            }
                            case MODE_VIEW:{
                                user.nowIndex -= (user.nowIndex > 0 ? 1 : 0);
                                break;
                            }
                            case CLEAR_COUNT_VIEW : {
                                user.sensor.liter = 0;
                                user.nowPage = user.lastPage;
                                break;
                            }
                            case INJECTION_PER_HOUR_VIEW : {
                                if(user.motor.injectionPerHour < 5000)
                                    user.motor.injectionPerHour += 10;
                                break;
                            }
                            case LOG_VIEW : {
                                user.historyIndex -= (user.historyIndex > 0 ? 1 : 0);
                                break;
                            }
                            default:
                                break;
                        }
                        update();
                        break;
                    }
                    case MODE : {
                        switch(user.nowPage){
                            case LOG_VIEW : {
                                user.nowPage = MAIN_VIEW;
                                user.mode = NOTHING_MODE;
                                user.historyIndex = -1;
                                delete user.history;
                                break;
                            }
                            case MAIN_VIEW : {
                                if(diffTime < 3000){
                                    user.mode = NOTHING_MODE;
                                    user.lastPage = user.nowPage;
                                    user.nowPage = MODE_VIEW;
                                    user.nowIndex = 0;
                                    user.itemLength = 4;
                                }else{
                                    user.mode = NOTHING_MODE;
                                    user.nowPage = SELECT_SENSOR_VIEW;
                                }
                                break;
                            }
                            case MODE_VIEW : {
                                user.mode = NOTHING_MODE;
                                user.lastPage = user.nowPage = MAIN_VIEW;
                                break;
                            }
                        }
                        break;
                    }
                    case DOWN:{
                        switch(user.nowPage){
                            case SET_CURRENT_TIME_VIEW : {
                                switch(user.time.index){
                                    case TIME_YEAR : {
                                        user.time.time.year--;
                                        break;
                                    }
                                    case TIME_MONTH : {
                                        user.time.time.month -= (user.time.time.month > 1 ? 1 : 0);
                                        break;
                                    }
                                    case TIME_DAY : {
                                        user.time.time.day -= (user.time.time.day > 1 ? 1 : 0);
                                        break;
                                    }
                                    case TIME_HOUR : {
                                        user.time.time.hour -= (user.time.time.hour > 0 ? 1 : 0);
                                        break;
                                    }
                                    case TIME_MINUTE : {
                                        user.time.time.minute -= (user.time.time.minute > 0 ? 1 : 0);
                                        break;
                                    }
                                }
                                break;
                            }
                            case SELECT_LOAD_DRUG_VIEW : {
                                user.nowPage = RUNNING_IN_MODE_VIEW;
                                break;
                            }
                            case SELECT_SENSOR_VIEW : {
                                if(user.sensor.sensorType > 0)
                                    user.sensor.sensorType--; 
                                break;
                            }
                            case SET_SCALE_VIEW : {
                                user.motor.scale -= (user.motor.scale > 0 ? 1 : 0);
                                break;
                            }
                            case MODE_VIEW : {
                                user.nowIndex += (user.nowIndex < (user.itemLength - 1) ? 1 : 0);
                                break;
                            }
                            case INJECTION_PER_HOUR_VIEW : {
                                if(user.motor.injectionPerHour > 0)
                                    user.motor.injectionPerHour -= 10;
                                break;
                            }
                            case CLEAR_COUNT_VIEW : {
                                user.nowPage = user.lastPage;
                                break;
                            }
                            case LOG_VIEW : {
                                user.historyIndex += (user.historyIndex < (user.history->length) ? 1 : 0);
                                break;
                            }
                            default : {
                                break;
                            }
                        }
                        update();
                        break;
                    }
                    case SCALE : {
                        if(user.nowPage == MAIN_VIEW){
                            user.nowPage = SET_SCALE_VIEW;
                        }
                        break;
                    }
                    case SAVE : {
                        switch(user.nowPage){
                            case SET_CURRENT_TIME_VIEW : {
                                switch(user.time.index){
                                    case TIME_YEAR : {
                                        user.time.index = TIME_MONTH;
                                        break;
                                    }
                                    case TIME_MONTH : {
                                        user.time.index = TIME_DAY;
                                        break;
                                    }
                                    case TIME_DAY : {
                                        user.time.index = TIME_HOUR;
                                        break;
                                    }
                                    case TIME_HOUR : {
                                        user.time.index = TIME_MINUTE;
                                        break;
                                    }
                                    case TIME_MINUTE : {
                                        user.time.index = TIME_DONE;
                                        break;
                                    }
                                    case TIME_DONE : {                                   
                                        timeSet(user.time.time);
                                        break;
                                    }
                                }
                                break;
                            }
                            case SET_SCALE_VIEW : {
                                user.motor.type = RUN_BY_SCALE;
                                user.mode = SCALE_MODE;
                                user.nowPage = RUNNING_IN_MODE_VIEW;
                                break;
                            }
                            case INJECTION_PER_HOUR_VIEW : {
                                user.nowPage = SELECT_LOAD_DRUG_VIEW;
                                break;
                            }
                            case SELECT_SENSOR_VIEW : {
                                user.nowPage = MAIN_VIEW;
                                break;
                            }
                            case MODE_VIEW : {
                                Serial.print("[index]");
                                Serial.println(user.nowIndex);
                                switch(user.nowIndex){

                                    //시간당 주입
                                    case 0 :{
                                        user.motor.type = RUN_BY_INJECTION_PER_HOUR;
                                        user.motor.injectionPerHour = 0;
                                        user.mode = INJECTION_PER_HOUR_MODE;
                                        user.nowPage = INJECTION_PER_HOUR_VIEW;
                                        break;
                                    }
                                    
                                    //음수량만 측정
                                    case 1 : {
                                        user.motor.type = NOT_RUN;
                                        user.motor.scale = 0;
                                        user.mode = MEASURE_ONLY_WATER_MODE;
                                        user.nowPage = RUNNING_IN_MODE_VIEW;
                                        break;
                                    }

                                    //날짜별 음수량
                                    case 2 : {
                                        user.nowPage = LOG_VIEW;
                                        user.mode = LOG_VIEW_MODE;
                                        user.historyIndex = -1;
                                        user.history = storage.get();
                                        break;
                                    }
                                    
                                    //현재시간 설정
                                    case 3 : {
                                        user.nowPage = SET_CURRENT_TIME_VIEW;
                                        user.mode = SET_CURRENT_TIME_MODE;
                                        user.time.index = TIME_YEAR;
                            
                                        #ifdef NOT_USE_RTC 
                                            user.time.time.year = 2018;
                                            user.time.time.month = 5;
                                            user.time.time.day = 2;
                                            user.time.time.hour = 20;
                                            user.time.time.minute = 59;
                                        #else
                                            time_t* temp = timeGet();
                                            memcpy(&user.time.time, temp, sizeof(time_t));
                                            delete temp;
                                        #endif
                                        break;
                                    }
                                    
                                }
                                break;
                            }
                        }
                        break;
                    }
                    case ZERO : {
                        //3초 이상 누른 경우
                        if(diffTime>3000){
                            user.lastPage = user.nowPage;
                            user.nowPage = CLEAR_COUNT_VIEW;
                        }
                        break;
                    }
                }
            }
            buttons[i].lastTime = millis();
            buttons[i].lastState = state;
        }
    }
    if(beforePage != user.nowPage){
        update();
    }
    if(checker.sensorType != user.sensor.sensorType |
        checker.type != user.motor.type |
        checker.injectionPerHour != user.motor.injectionPerHour){
        eeprom_setting_t* saveData = new eeprom_setting_t;
        saveData->injectionPerHour = user.motor.injectionPerHour;
        saveData->type = user.motor.type;
        saveData->sensorType = user.sensor.sensorType;
        storage.saveSetting(saveData);
        delete saveData;
    }
}