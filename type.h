#ifndef __TYPE_H__
#define __TYPE_H__

extern "C" {
    #include <inttypes.h>
}

// 사용처_뭐에 대한_t

typedef struct {
    uint16_t year;    

    uint16_t month : 4;
    uint16_t day : 5;
    uint16_t hour : 6;
    uint16_t am_pm : 1; // 1 PM, 0 AM

    uint8_t minute;

    uint8_t second;
} eeprom_time_t;

typedef struct {
    uint32_t liter;
    uint16_t mill_liter;
} eeprom_liquid_amount_t;

typedef struct {
    uint8_t index;
    eeprom_time_t time;
    eeprom_liquid_amount_t amount;
} eeprom_data_item_t;

typedef struct {
    uint64_t counter;
    uint64_t tick;

    inline sensor_data_t operator++(){
        tick++;
        if(tick == UINT64_MAX){
            counter++;
            tick = 0;
        }
    }
} sensor_data_t;

typedef struct __FRAME_T{
    uint16_t width, height;
    uint8_t* serializeFrame;
    uint8_t** frameBuffer;

    __FRAME_T(){
        width = 128;
        height = 64;
        serializeFrame = new uint8_t[width * height];
        frameBuffer = new uint8_t[width];
        for(uint16_t i = 0;i<width;i++){
            frameBuffer[i] = serializeFrame[i * height];
        }
    };

} screen_frame_t;
typedef struct {
    uint8_t length;
    eeprom_data_item_t* items;
    inline eeprom_data_item_t operator[](const int index){
        return items[index];
    };
} eeprom_data_list_t;

eeprom_liquid_amount_t literToAmount(double);

#endif