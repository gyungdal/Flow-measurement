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

//한글은 한 글자당 12, Space는 4

//u8glib는 글자를 그릴때에는 끝점부터잡고, xbm은 시작점을 잡는듯

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
    user.sensor.sensorType = 0;
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

    update();
    #ifdef DEBUG
        Serial.println("[DEBUG] DRAW SETUP");
    #endif
}

//메인
static void mainViewDraw(){
    char str[100];
    u8g.firstPage();
    while(u8g.nextPage()){
        u8g.drawXBM(0, 6, WATER_SCALE_XBM.width, WATER_SCALE_XBM.height, WATER_SCALE_XBM.value);
        u8g.drawXBM(0, 19, WATER_SPEED_XBM.width, WATER_SPEED_XBM.height, WATER_SPEED_XBM.value);
        u8g.drawXBM(0, 32, WATER_COUNT_XBM.width, WATER_COUNT_XBM.height, WATER_COUNT_XBM.value);
        u8g.drawXBM(0, 45, SENSOR_XBM.width, SENSOR_XBM.height, SENSOR_XBM.value);

        u8g.setFont(u8g_font_7x14);
        sprintf(str, " : 1/%d", user.motor.getScale());
        u8g.drawStr(24, 18, str);
        memset(str, 0x00, 100);
        sprintf(str, " : %dL/h", user.sensor.waterPerHour);
        u8g.drawStr(24, 31, str);
        memset(str, 0x00, 100);
        sprintf(str, " : %dL", user.sensor.liter);
        u8g.drawStr(36, 44, str);
        memset(str, 0x00, 100);
        sprintf(str, " : %u", user.sensor.sensorType);
        u8g.drawStr(24, 57, str);   
    }
}

//메뉴 
static void menuViewDraw(){
    u8g.firstPage();
    xbm_t* list = new xbm_t[4];
    list[0] = INJECTION_PER_HOUR_XBM;
    list[1] = MEASURE_ONLY_WATER_XBM;
    list[2] = AMOUNT_BY_DAY_XBM;
    list[3] = SET_CURRENT_TIME_XBM;

    while(u8g.nextPage()){
        for(uint8_t i = 0;i < user.itemLength + 1;i++){
            u8g.setDefaultForegroundColor();
            if(user.nowIndex == i){
                u8g.drawBox(0, 6 + (13 * i), 128, 13);
                u8g.setDefaultBackgroundColor();
            }
            u8g.drawXBM(0, 6 + (13 * i), list[i].width, list[i].height, list[i].value);
        }
    }

    delete[] list;
}

//시간당 비율 설정 뷰
static void scaleViewDraw(){

}

//~~모드로 동작
static void runningViewDraw(){

}

//장전중
static void loadingViewDraw(){
    xbm_t* list = new xbm_t[2];
    list[0] = INJECTION_PER_HOUR_XBM;
    list[1] = MEASURE_ONLY_WATER_XBM;
    u8g.firstPage();
    while(u8g.nextPage()){
        u8g.drawXBM(0, 19, list[user.nowIndex].width, list[user.nowIndex].height, list[user.nowIndex].value);
        u8g.drawXBM(0, 32, RUNNING_IN_MODE_XBM.width, RUNNING_IN_MODE_XBM.height, RUNNING_IN_MODE_XBM.value);
    }
    delete[] list;
}

void update(){
    switch(user.nowPage){
        case MAIN_VIEW:
            mainViewDraw();
            break;
        case MODE_VIEW:
            menuViewDraw();
            break;
        case CLEAR_COUNT_VIEW: {
            u8g.firstPage();
            while(u8g.nextPage()){
                u8g.setFont(u8g_font_unifont);
                //u8g.setFont(u8g_font_osb21);
                u8g.drawXBM(0, 0, CLEAR_COUNT_XBM.width, CLEAR_COUNT_XBM.height, CLEAR_COUNT_XBM.value);
                u8g.drawXBM(0, 24, YES_XBM.width, YES_XBM.height, YES_XBM.value);
                u8g.drawXBM(0, 48, NO_XBM.width, NO_XBM.height, NO_XBM.value);
            }
            break;
        }
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
    display_menu_t beforePage = user.nowPage;
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
                            case MODE_VIEW:{
                                user.nowIndex -= (user.nowIndex > 0 ? 1 : 0);
                                break;
                            }
                            case CLEAR_COUNT_VIEW : {
                                user.nowPage = user.lastPage;
                                break;
                            }
                            default:
                                break;
                        }
                        update();
                        break;
                    }
                    case MODE: {
                        switch(user.nowPage){
                            case MAIN_VIEW : {
                                user.lastPage = user.nowPage;
                                user.nowPage = MODE_VIEW;
                                user.nowIndex = -1;
                                user.itemLength = 3;
                                break;
                            }
                            case MODE_VIEW : {
                                switch(user.nowIndex){
                                    case -1 :{
                                        user.nowPage = MAIN_VIEW;
                                        user.nowIndex = user.itemLength = 0;
                                        break;
                                    }

                                    //시간당 주입
                                    case 0 :{

                                        break;
                                    }
                                    
                                    //음수량만 측정
                                    case 1 : {
                                        
                                        break;
                                    }
                                    
                                    //날짜별 음수량
                                    case 2 : {

                                        break;
                                    }
                                    
                                    //현재시간 설정
                                    case 3 : {

                                        break;
                                    }
                                    
                                }
                            }
                        }
                        break;
                    }
                    case DOWN:{
                        switch(user.nowPage){
                            case MODE_VIEW:{
                                user.nowIndex += (user.nowIndex < user.itemLength ? 1 : 0);
                                break;
                            }
                            case CLEAR_COUNT_VIEW : {
                                user.nowPage = user.lastPage;
                                break;
                            }
                            default:
                                break;
                        }
                        update();
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
                update();
            }
            buttons[i].lastTime = millis();
            buttons[i].lastState = state;
        }
    }
    if(beforePage != user.nowPage)
        update();
}
