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
    eeprom_time_t time;
    eeprom_liquid_amount_t amount;
} eeprom_data_item_t;

typedef struct {
    uint8_t length;
    eeprom_data_list_t* items;
    inline eeprom_data_list_t operator[](const int index){
        return items[index];
    };
} eeprom_data_list_t;

eeprom_liquid_amount_t literToAmount(double);
#endif