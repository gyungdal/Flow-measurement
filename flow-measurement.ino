#include "U8glib.h"

#include "module/korean.h"
#include "module/type.h"
#include "module/storage.h"

U8GLIB_ST7920_128X64_1X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17

//font 한개 사이즈 : 11

button_t buttons[] = {
    { .type = SCALE, .pin = 22, .lastState = LOW, .lastTime = 0},
    { .type = SAVE, .pin = 23, .lastState = LOW, .lastTime = 0},
    { .type = ZERO, .pin = 24, .lastState = LOW, .lastTime = 0},
    { .type = MODE, .pin = 25, .lastState = LOW, .lastTime = 0},
    { .type = UP, .pin = 26, .lastState = LOW, .lastTime = 0},
    { .type = DOWN, .pin = 27, .lastState = LOW, .lastTime = 0}
};

void setup() {
    nowIndex = 0;
    nowMenu = MAIN_VIEW;
    for(uint8_t i = 0;i<sizeof(buttons) / sizeof(button_t);i++){
        pinMode(buttons[i].pin, INPUT);
    }
    Serial.begin(115200);
    u8g.firstPage();
    while(u8g.nextPage()){
        u8g.drawXBM(0, 24, YES_XBM.width, YES_XBM.height, YES_XBM.value);
    }
    //update();
}

void update(){

    switch(nowMenu){
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
        Serial.print(buttons[i].pin);
        Serial.println(state ? "HIGH" : "LOW");
        if(state != buttons[i].lastState){
            if(buttons[i].lastState == LOW){
                switch(buttons[i].type){
                    case UP :{
                        switch(nowMenu){
                            case MAIN_VIEW:{
                                nowIndex -= (nowIndex > 0 ? 1 : 0);
                                break;
                            }
                        }
                        break;
                    }
                    case DOWN:{
                        switch(nowMenu){
                            case MAIN_VIEW:{
                                nowIndex += (nowIndex < 4 ? 1 : 0);
                                break;
                            }
                        }
                        break;
                    }
                }
                update();
            }
            buttons[i].lastState = state;
        }
    }
}
