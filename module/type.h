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
    uint8_t second;
} time_t;

typedef struct {
    uint32_t liter;
    uint16_t milliLiter;
} liquid_amount_t;

typedef struct {
    uint8_t index;
    time_t time;
    liquid_amount_t amount;
} eeprom_item_t;

typedef struct {
    uint8_t length;
    eeprom_item_t* items;
    inline eeprom_item_t operator[](const int index){
        return items[index];
    };
} eeprom_list_t;

//센서별 리터당 틱
static const int sensor_tick[] = {   
    0, 0, 0, 0, 0, 0, 0  
};

//sensor data
typedef struct {
    uint8_t sensorType;
    int pin;
    uint64_t liter;
    uint32_t tick;
    uint32_t waterPerHour;

    inline void resetAmount(){
        liter = 0;
        tick = 0;
    }

    inline liquid_amount_t getAmount(){
        liquid_amount_t result;
        result.liter = liter;
        result.milliLiter = (uint16_t)(((double)sensor_tick[sensorType] / tick) * 1000);
        return result;
    }

    inline void operator++(){
        tick++;
        if(tick == sensor_tick[sensorType]){
            liter++;
            tick = 0;
        }
    }
} sensor_t;

//motor 
static const uint32_t motor_scale_list[] = {
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

typedef enum {
    RUN_BY_SCALE,
    RUN_BY_INJECTION_PER_HOUR,
    NOT_RUN
} motor_run_type_t;

typedef struct {
    motor_run_type_t type;
    union {
        uint32_t injectionPerHour;
        uint32_t scale;
    };
    
    inline uint32_t getScale(){
        return motor_scale_list[scale];
    }
    bool isError;
    uint8_t angle; // 0 : 0, 1 : 120, 2 : 240
    uint16_t tick;
    uint64_t lastTime;
    int pwmPin;
    int signalPin[3];
} motor_t;

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
    uint64_t lastTime;
} button_t;

//display data
typedef enum {
    MAIN_VIEW, //메인 뷰
    MODE_VIEW, //모드 뷰 페이지
    SET_SCALE_VIEW, //약물 비율 설정
    SELECT_LOAD_DRUG_VIEW, //약물 장전 선택 페이지
    LOADING_DRUG_VIEW, //약물 장전중 페이지
    RUNNING_IN_MODE_VIEW, //~~~ 모드로 동작
    INJECTION_PER_HOUR_VIEW, //시간당 주입량 설정 페이지
    LOG_VIEW, //이전 기록뷰
    SET_CURRENT_TIME_VIEW, //현재 시간 설정 페이지
    CLEAR_COUNT_VIEW, //누적량 초기화 페이지
    SELECT_SENSOR_VIEW //센서 선택 페이지
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

typedef enum {
    INJECTION_PER_HOUR_MODE,
    SCALE_MODE,
    MEASURE_ONLY_WATER_MODE,
    NOTHING_MODE
} mode_t;

typedef struct {
    display_menu_t lastPage;
    display_menu_t nowPage;
    mode_t mode;
    int8_t nowIndex;
    uint8_t itemLength;
    int alertPin;
    motor_t motor;
    sensor_t sensor;
} user_t;

#endif  