#include "U8glib.h"

#include "module/type.h"
#include "module/storage.h"

U8GLIB_ST7920_128X64_1X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
display_menu_t nowMenu;
int nowIndex;
display_data_list_t frame;
//font 한개 사이즈 : 11

button_data_t buttons[] = {
    { .type = SCALE, .pin = 22, .lastState = LOW, .lastTime = 0},
    { .type = SAVE, .pin = 23, .lastState = LOW, .lastTime = 0},
    { .type = ZERO, .pin = 24, .lastState = LOW, .lastTime = 0},
    { .type = MODE, .pin = 25, .lastState = LOW, .lastTime = 0},
    { .type = UP, .pin = 26, .lastState = LOW, .lastTime = 0},
    { .type = DOWN, .pin = 27, .lastState = LOW, .lastTime = 0}
};

static void mainViewSetup(){
    
    frame.items = new display_data_item_t[5];
    frame.length = 5;
    const char* names[] = {
        "Scale\0",
        "Speed\0",
        "Count\0",
        "Sensor\0",
        "Alarm\0"
    };
    for(uint8_t i = 0;i<frame.length;i++){
        memset(frame.items[i].name, 0x00, 20);
        memset(frame.items[i].str, 0x00, 40);
        strcpy(frame.items[i].name, names[i]);
    }
    sprintf(frame.items[0].str, "1/%lu", scaleList[73]);
    sprintf(frame.items[1].str, "%lu/h", scaleList[73]);
    sprintf(frame.items[2].str, "%luL", scaleList[73]);
    sprintf(frame.items[3].str, "%lu", scaleList[0]);
    sprintf(frame.items[4].str, "Motor");
    frame.items[0].nowSelect = frame.items[1].nowSelect = frame.items[2].nowSelect 
            = frame.items[3].nowSelect = frame.items[4].nowSelect = false;
    frame.items[nowIndex].nowSelect = true;
}

void setup() {
    nowIndex = 0;
    nowMenu = MAIN_VIEW;
    for(uint8_t i = 0;i<sizeof(buttons) / sizeof(button_data_t);i++){
        pinMode(buttons[i].pin, INPUT);
    }
    Serial.begin(115200);
    update();
}

void update(){
    delete[] frame.items;

    switch(nowMenu){
        case MAIN_VIEW:
            mainViewSetup();
            break;
        default:
            break;
    }
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
            sprintf(str, "%s : %s", frame[i].name, frame[i].str);
            d = (w-u8g.getStrWidth(str))/2;
            u8g.setDefaultForegroundColor();
            if (frame.items[i].nowSelect) {
                u8g.drawBox(0, space + i * h + 1, w, h);
                u8g.setDefaultBackgroundColor();
            }
            u8g.drawStr(d, space + i * h, str);
        }
        delete[] str;
    }
}

void loop() {  
    for(uint8_t i = 0;i<sizeof(buttons) / sizeof(button_data_t);i++){
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
                frame.items[0].nowSelect = !frame.items[0].nowSelect;
                update();
            }
            buttons[i].lastState = state;
        }
    }
}
