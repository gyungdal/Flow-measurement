#include "U8glib.h"
#include <TimerOne.h>

#include "module/korean.h"
#include "module/type.h"
#include "module/storage.cpp"
#include "module/rtc.cpp"

/*
    U8GLIB_ST7920_128X64_1X(sck, mosi, cs [, reset])
    Serial mode (PSB = LOW)
    sck: Pin with label "E"
    mosi: Pin with label "RW"
    cs: Pin with label "RS".
    Example: U8GLIB_ST7920_128X64_1X(sck, mosi, cs [, reset]) is equivalent to U8GLIB_ST7920_128X64_1X(E, RW, RS, RST) for many display modules. 
*/

//SW SPI-> HW SPI로 변경
U8GLIB_ST7920_128X64_1X u8g(49);

const int RTC_PIN = 13;
//font 한개 사이즈 : 11

Storage storage;

user_t user;

button_t buttons[] = {
    { .type = SCALE, .pin = 22, .lastState = LOW},
    { .type = SAVE, .pin = 23, .lastState = LOW},
    { .type = ZERO, .pin = 24, .lastState = LOW},
    { .type = MODE, .pin = 25, .lastState = LOW},
    { .type = UP, .pin = 26, .lastState = LOW},
    { .type = DOWN, .pin = 27, .lastState = LOW}
};
/*
static void dayHandler(){
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

    time_t* time = rtc.get();
    list->items[list->length].time.year = time->year;
    list->items[list->length].time.month = time->month;
    list->items[list->length].time.day = time->day;
    list->items[list->length].time.hour = time->hour;
    list->items[list->length].time.minute = time->minute;
    delete time;

    list->length += 1;
    storage.set(list);

    delete[] list->items;
    delete list;
}
*/
static liquid_amount_t lastAmount;

static void secondTimer(){
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

void setup() {
    Serial.begin(115200);
    //EEPROM 정리
    #ifdef EEPROM_CLEAR
    storage.clear();
    #endif

    //1초 타이머 설정
    //Timer1.initialize(1000000);
    //Timer1.attachInterrupt(secondTimer);

    //RTC에서 오는 1일 마다 발생하는 타이머 설정 
    //rtc.setDayHandler(dayHandler);

    //현재 페이지 설정 (MAIN_VIEW)
    user.lastPage = MAIN_VIEW;
    user.nowPage = MAIN_VIEW;
    for(uint8_t i = 0;i<sizeof(buttons) / sizeof(button_t);i++){
        pinMode(buttons[i].pin, INPUT);
    }
    u8g.firstPage();
    while(u8g.nextPage()){
        u8g.setFont(u8g_font_unifont);
        //u8g.setFont(u8g_font_osb21);
        u8g.drawStr(0, 48, "Hello World!");
        u8g.drawXBM(0, 24, YES_XBM.width, YES_XBM.height, YES_XBM.value);
    }
    //update();
    Serial.println("[DEBUG] DRAW SETUP");
}

void update(){
    switch(user.nowPage){
        case MAIN_VIEW:
            break;
        default:
            break;
    }/*
    u8g.firstPage();
    while(u8g.nextPage()){
        uint8_t i, h;
        u8g_uint_t w, d;

        u8g.setFont(u8g_font_6x13);
        u8g.setFontRefHeightText();
        u8g.setFontPosTop();

        h = u8g.getFontAscent() - u8g.getFontDescent();
        w = u8g.getWidth();

        uint8_t space = (64 - (h * frame.length));
        char* str = new char[100];
        for( i = 0; i < frame.length; i++) {
            memset(str, 0x00, sizeof(str) / sizeof(char));
            //sprintf(str, "%s : %s", frame[i].name, frame[i].str);
            d = (w-u8g.getStrWidth(str))/2;
            u8g.setDefaultForegroundColor();
            if (frame.items[i].nowSelect) {
                u8g.drawBox(0, space + i * h + 1, w, h);
                u8g.setDefaultBackgroundColor();
            }
            u8g.drawStr(d, space + i * h, str);
        }
        delete[] str;
    }*/
}

void loop() {  
    for(uint8_t i = 0;i<sizeof(buttons) / sizeof(button_t);i++){
        int state = digitalRead(buttons[i].pin);
        if(state != buttons[i].lastState){
            if(buttons[i].lastState != LOW){
                uint64_t diffTime = millis() - buttons[i].lastTime;
                if(diffTime < 120)
                    continue;
                #define DEBUG
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
                            case MAIN_VIEW:{
                                user.nowIndex -= (user.nowIndex > 0 ? 1 : 0);
                                break;
                            }
                        }
                        break;
                    }
                    case DOWN:{
                        switch(user.nowPage){
                            case MAIN_VIEW:{
                                user.nowIndex += (user.nowIndex < 4 ? 1 : 0);
                                break;
                            }
                        }
                        break;
                    }
                }
                update();
            }
            buttons[i].lastTime = millis();
            buttons[i].lastState = state;
        }
    }
}
