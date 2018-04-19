#ifndef __TYPE_H__
#define __TYPE_H__

extern "C" {
    #include <inttypes.h>
}

// 사용처_뭐에_대한_t

//eeprom data
typedef struct {
    uint16_t year;    

    uint16_t month : 5;
    uint16_t day : 6;
    uint16_t hour : 5;

    uint8_t minute;
} time_data_t;

typedef struct {
    uint32_t liter;
    uint16_t millLiter;
} eeprom_liquid_amount_t;

typedef struct {
    uint8_t index;
    time_data_t time;
    eeprom_liquid_amount_t amount;
} eeprom_data_item_t;


typedef struct {
    uint8_t length;
    eeprom_data_item_t* items;
    inline eeprom_data_item_t operator[](const int index){
        return items[index];
    };
} eeprom_data_list_t;

//sensor data
typedef struct _SENSOR_DATA {
    uint64_t counter;
    uint64_t tick;

    inline struct _SENSOR_DATA operator++(){
        tick++;
        if(tick == UINT64_MAX){
            counter++;
            tick = 0;
        }
    }
} sensor_data_t;

//button
typedef enum {
    SCALE,
    SAVE,
    ZERO,
    MODE,
    UP,
    DOWN
} button_type_t;

typedef struct { 
    button_type_t type;
    int pin;
    int lastState;
    long lastTime;
} button_data_t;

//display data
typedef enum {
    MAIN_VIEW,
    MODE_VIEW,
    SET_SCALE_VIEW,
    SELECT_LOAD_DRUG_VIEW,
    LOADING_DRUG_VIEW,
    RUNNING_MODE_VIEW,
    INJECTION_PER_HOUR_VIEW,
    LOG_VIEW,
    SET_CURRENT_TIME_VIEW,
    RESET_COUNT_VIEW
} display_menu_t;

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t* value;
} xbm_t;

/*
typedef enum {
    CLEAR_COUNT_XBM, //누적량 초기화
    AMOUNT_BY_DAY_XBM, //날짜별 음수량
    COUNT_XBM, //누적량
    WATER_OVERFLOW_XBM, //물량 과다
    SCALE_XBM, //비율
    SAVE_AFTER_SETTING_XBM, //설정 후 저장
    SENSOR_TYPE_XBM, //센서 종류
    INJECTION_PER_HOUR_XBM, //시간당 주입
    PER_HOUR_XBM, //시간당
    NO_XBM, //아니오
    LOADING_DRUG_XBM, //약물 장전 중
    YES_XBM, //예
    MEASURE_ONLY_WATER_XBM, //음수량만 측정
    SET_CURRENT_TIME_XBM, //현재 시간 설정
    WATER_SPEED_XBM, //유속
    CHECK_TUBE_XBM, //튜브 점검
    STOP_XBM, //정지
} xbm_type_t;
*/

const uint32_t scaleList[] = {
    33, 35, 40, 45, 50, 60, 70, 80, 90,
    100, 110, 120, 130, 140, 150, 160, 170, 180, 190,
    200, 250, 300, 350, 400, 450,
    500, 600, 700, 800, 900, 
    1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700, 1800, 1900,			
    2000, 2500, 3000, 3500, 4000, 4500,
    5000, 6000, 7000, 8000, 9000,
    10000, 11000, 12000, 13000, 14000, 15000, 16,000, 17000, 18000, 19000,	
    20000, 25000, 30000, 35000, 40000, 45000,
    50000, 60000, 70000, 80000, 90000, 100000
};

typedef struct { 
    uint8_t scaleIndex;
    uint32_t getScale(){
        return scaleList[scaleIndex];
    };


} setting_data_t;

typedef struct {
    display_menu_t lastPage;
    display_menu_t nowPage;
    setting_data_t setting;
} user_data_t;
//function
eeprom_liquid_amount_t literToAmount(double);

#endif  