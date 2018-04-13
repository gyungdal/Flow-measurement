#include "U8glib.h"

U8GLIB_ST7920_128X64_1X u8g(8, 9, 10);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
display_menu_t nowMenu;

const button_data_t buttons[] = {
    { .type = SCALE, .pin = 22},
    { .type = SAVE, .pin = 23},
    { .type = ZERO, .pin = 24},
    { .type = MODE, .pin = 25},
    { .type = UP, .pin = 26},
    { .type = DOWN, .pin = 27}
};

void drawMenu(void) {
    uint8_t i, h;
    u8g_uint_t w, d;

    u8g.setFont(u8g_font_6x13);
    u8g.setFontRefHeightText();
    u8g.setFontPosTop();

    h = u8g.getFontAscent()-u8g.getFontDescent();
    w = u8g.getWidth();
    for( i = 0; i < MENU_ITEMS; i++ ) {
        d = (w-u8g.getStrWidth(menu_strings[i]))/2;
        u8g.setDefaultForegroundColor();
        if ( i == menu_current ) {
            u8g.drawBox(0, i*h+1, w, h);
            u8g.setDefaultBackgroundColor();
        }
        u8g.drawStr(d, i*h, menu_strings[i]);
    }
}

void setup() {
    Serial.begin(115200);
}

void loop() {  
    u8g.firstPage();
    do  {
        drawMenu();
    } while( u8g.nextPage() );
}
