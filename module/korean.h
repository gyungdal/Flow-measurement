#ifndef __KOREAN_H__
#define __KOREAN_H__

#include "type.h"

//날짜별 음수량
static uint8_t AMOUNT_BY_DAY[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0xe2,
    0x2f, 0x42, 0x02, 0xf8, 0x01, 0x04, 0x7e, 0x02, 0x02, 0x82, 0x22, 0xc2,
    0x03, 0x04, 0x02, 0x04, 0x40, 0x0e, 0x02, 0x8e, 0x22, 0x7e, 0x02, 0x04,
    0x02, 0x0a, 0x7e, 0x02, 0x02, 0x82, 0x22, 0xc2, 0x03, 0xf8, 0x01, 0x11,
    0x02, 0x02, 0xfe, 0x82, 0x22, 0x7e, 0x02, 0x00, 0xc0, 0x60, 0x02, 0x0e,
    0x00, 0x40, 0xe3, 0x00, 0x02, 0xfe, 0x07, 0x00, 0xfe, 0x02, 0xfc, 0x43,
    0x25, 0xfc, 0x03, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x22, 0x25, 0x00,
    0x02, 0xfc, 0xe3, 0x7f, 0xf8, 0x01, 0xfc, 0xa3, 0x28, 0xfc, 0x03, 0x04,
    0x02, 0x04, 0x04, 0x02, 0x04, 0x80, 0x20, 0x04, 0x00, 0x04, 0x02, 0x04,
    0x04, 0x02, 0xfc, 0x03, 0x20, 0xfc, 0x03, 0xfc, 0x03, 0x04, 0xf8, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t AMOUNT_BY_DAY_XBM = {
    .width = 76,
    .height = 13,
    .value = AMOUNT_BY_DAY
};

//누적량 초기화
static uint8_t CLEAR_COUNT[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xe0,
    0x27, 0x7e, 0x02, 0xe0, 0xe0, 0x27, 0x38, 0x02, 0x04, 0x80, 0x20, 0x40,
    0x0e, 0x00, 0x00, 0x24, 0x00, 0x02, 0x04, 0x80, 0x3c, 0x7e, 0x02, 0xf8,
    0x03, 0x24, 0xfe, 0x02, 0xfc, 0x83, 0x21, 0x02, 0x02, 0x40, 0x00, 0x24,
    0x00, 0x02, 0x00, 0x40, 0x22, 0x02, 0x0e, 0xa0, 0x00, 0x24, 0x38, 0x02,
    0x00, 0x20, 0x24, 0xfe, 0x02, 0x10, 0x01, 0x22, 0x44, 0x0e, 0xfe, 0x07,
    0x00, 0x00, 0x02, 0x0c, 0x02, 0x22, 0x44, 0x02, 0x40, 0xc0, 0x3f, 0xf8,
    0x01, 0x40, 0x00, 0x21, 0x38, 0x02, 0x40, 0x00, 0x20, 0x04, 0x02, 0x40,
    0x80, 0x20, 0x10, 0x02, 0x40, 0x00, 0x20, 0x04, 0x02, 0xfe, 0x67, 0x20,
    0xfe, 0x02, 0x40, 0x00, 0x20, 0xf8, 0x01, 0x00, 0x00, 0x20, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t CLEAR_COUNT_XBM = {
    .width = 76,
    .height = 13,
    .value = CLEAR_COUNT
};

//모드로 동작합니다
static uint8_t RUNNING_IN_MODE[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xfc, 0xc3, 0x3f, 0xfc, 0x03, 0xfc, 0xe3, 0x2f, 0x1c, 0x22, 0x20,
    0x7e, 0x02, 0x04, 0x42, 0x00, 0x00, 0x02, 0x04, 0x00, 0x21, 0x00, 0x22,
    0x20, 0x02, 0x02, 0x04, 0x42, 0x00, 0x00, 0x02, 0x04, 0x00, 0x21, 0x7f,
    0x22, 0x20, 0x02, 0x02, 0x04, 0x42, 0x00, 0xfc, 0x03, 0xfc, 0x83, 0xe2,
    0x14, 0x2e, 0x20, 0x02, 0x02, 0x04, 0x42, 0x00, 0x04, 0x00, 0x40, 0x40,
    0x24, 0x22, 0x22, 0x20, 0x02, 0x02, 0xfc, 0xc3, 0x3f, 0x04, 0x00, 0xfe,
    0x27, 0x28, 0x22, 0x22, 0x20, 0x02, 0x0e, 0x40, 0x00, 0x00, 0xfc, 0x03,
    0x00, 0x00, 0x00, 0x1c, 0x20, 0x20, 0x02, 0x02, 0x40, 0x00, 0x00, 0x40,
    0x00, 0xf8, 0xc1, 0x3f, 0x04, 0x22, 0x20, 0x02, 0x02, 0x40, 0x00, 0x00,
    0x40, 0x00, 0x04, 0x02, 0x20, 0xfc, 0x23, 0x20, 0x02, 0x02, 0xfe, 0xe7,
    0x7f, 0xfe, 0x07, 0x04, 0x02, 0x20, 0x04, 0xe2, 0x2f, 0xfe, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xf8, 0x01, 0x20, 0xfc, 0x03, 0x20, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00
};

xbm_t RUNNING_IN_MODE_XBM = {
    .width = 100,
    .height = 13,
    .value = RUNNING_IN_MODE
};

//물량 과다
static uint8_t WATER_OVERFLOW[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xe3, 0x27, 0xe0, 0x27,
    0x7e, 0x02, 0x04, 0x02, 0xe4, 0x00, 0x24, 0x02, 0x02, 0xfc, 0xe3, 0x27,
    0x00, 0x24, 0x02, 0x02, 0x00, 0x20, 0x20, 0x00, 0x24, 0x02, 0x02, 0xfe,
    0x27, 0xe0, 0x80, 0x24, 0x02, 0x02, 0x40, 0xe0, 0x2f, 0x80, 0xe4, 0x02,
    0x0e, 0xfc, 0x03, 0x20, 0x80, 0x24, 0x02, 0x02, 0x00, 0x82, 0x1f, 0x80,
    0x22, 0x02, 0x02, 0xfc, 0x43, 0x20, 0x80, 0x20, 0x02, 0x02, 0x04, 0x40,
    0x20, 0xe0, 0x2f, 0xfe, 0x02, 0xfc, 0x83, 0x1f, 0x00, 0x20, 0x00, 0x02,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t WATER_OVERFLOW_XBM = {
    .width = 52,
    .height = 13,
    .value = WATER_OVERFLOW
};

//설정후 저장
static uint8_t SAVE_AFTER_SETTING[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xe2, 0x27,
    0x00, 0x0f, 0xe0, 0x2f, 0xfe, 0x02, 0x10, 0x82, 0x20, 0x00, 0x00, 0x00,
    0x21, 0x10, 0x02, 0xd8, 0x83, 0x3c, 0xe0, 0x7f, 0x00, 0x21, 0x10, 0x02,
    0x28, 0x82, 0x21, 0x00, 0x00, 0x00, 0x21, 0x28, 0x0e, 0x44, 0x42, 0x22,
    0x80, 0x1f, 0x00, 0x3d, 0x44, 0x02, 0x02, 0x20, 0x24, 0x40, 0x20, 0x80,
    0x22, 0x82, 0x02, 0xfc, 0x03, 0x00, 0x80, 0x1f, 0x80, 0x22, 0x00, 0x00,
    0x00, 0x82, 0x1f, 0x00, 0x00, 0x40, 0x24, 0xf8, 0x01, 0xfc, 0x43, 0x20,
    0xe0, 0x7f, 0x40, 0x24, 0x04, 0x02, 0x04, 0x40, 0x20, 0x00, 0x04, 0x20,
    0x28, 0x04, 0x02, 0xfc, 0x83, 0x1f, 0x00, 0x04, 0x00, 0x20, 0xf8, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t SAVE_AFTER_SETTING_XBM = {
    .width = 68,
    .height = 13,
    .value = SAVE_AFTER_SETTING
};

//센서 종류 선택
static uint8_t SELECT_SENSOR_TYPE[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x02,
    0x21, 0xc0, 0x3f, 0xfc, 0x03, 0x10, 0xe2, 0x25, 0x88, 0x02, 0x21, 0x00,
    0x06, 0x00, 0x02, 0x10, 0x22, 0x24, 0xec, 0x02, 0x21, 0x00, 0x09, 0xfc,
    0x03, 0xd0, 0xe3, 0x3d, 0x94, 0x02, 0x21, 0xc0, 0x30, 0x04, 0x00, 0x28,
    0x22, 0x24, 0x94, 0x02, 0x39, 0x00, 0x04, 0x04, 0x00, 0x48, 0x22, 0x24,
    0xa2, 0x82, 0x22, 0xe0, 0x7f, 0xfc, 0x03, 0x84, 0xe2, 0x25, 0x80, 0x82,
    0x22, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x80, 0x42, 0x24, 0x80,
    0x1f, 0xfe, 0x07, 0x00, 0xc2, 0x3f, 0x04, 0x40, 0x24, 0x40, 0x20, 0x08,
    0x01, 0x04, 0x00, 0x20, 0x04, 0x20, 0x28, 0x40, 0x20, 0x08, 0x01, 0x04,
    0x00, 0x20, 0xfc, 0x03, 0x20, 0x80, 0x1f, 0x08, 0x01, 0xfc, 0x03, 0x20,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

xbm_t SELECT_SENSOR_TYPE_XBM = {
    .width = 80,
    .height = 13,
    .value = SELECT_SENSOR_TYPE
};

//시간당 주입
static uint8_t INJECTION_PER_HOUR[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0xe2, 0x27, 0x7e,
    0x02, 0xfc, 0xc3, 0x23, 0x10, 0x02, 0x24, 0x02, 0x02, 0x40, 0x20, 0x24,
    0x10, 0x02, 0x24, 0x02, 0x02, 0xa0, 0x20, 0x24, 0x10, 0x02, 0xe4, 0x02,
    0x0e, 0x10, 0x21, 0x24, 0x10, 0x02, 0x22, 0xfe, 0x02, 0x0c, 0xc2, 0x23,
    0x28, 0x02, 0x21, 0x00, 0x02, 0x00, 0x00, 0x00, 0x28, 0xe2, 0x20, 0x00,
    0x00, 0x00, 0x40, 0x20, 0x44, 0x02, 0x20, 0xf8, 0x01, 0xfe, 0x47, 0x20,
    0x44, 0x42, 0x00, 0x04, 0x02, 0x40, 0xc0, 0x3f, 0x82, 0x42, 0x00, 0x04,
    0x02, 0x40, 0x40, 0x20, 0x00, 0xc2, 0x3f, 0xf8, 0x01, 0x40, 0xc0, 0x3f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t INJECTION_PER_HOUR_XBM = {
    .width = 64,
    .height = 13,
    .value = INJECTION_PER_HOUR
};

//아니오
static uint8_t NO[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x22, 0x20, 0xf0,
    0x00, 0x08, 0x80, 0x00, 0x42, 0x22, 0x20, 0x08, 0x01, 0x04, 0x02, 0x01,
    0x42, 0x22, 0x20, 0x04, 0x02, 0x04, 0x02, 0x01, 0x42, 0x22, 0x20, 0x04,
    0x02, 0x02, 0x02, 0x02, 0x42, 0x22, 0x20, 0x08, 0x01, 0x02, 0x02, 0x02,
    0x42, 0x2e, 0x20, 0xf0, 0x00, 0x02, 0x02, 0x02, 0x42, 0x22, 0x20, 0x40,
    0x00, 0x02, 0x02, 0x02, 0x42, 0x22, 0x20, 0x40, 0x00, 0x02, 0x02, 0x02,
    0x42, 0x22, 0x20, 0x40, 0x00, 0x84, 0x0a, 0x01, 0x3c, 0xe2, 0x2f, 0xfe,
    0x07, 0x04, 0x07, 0x01, 0x00, 0x02, 0x20, 0x00, 0x00, 0x08, 0x82, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t NO_XBM = {
    .width = 59,
    .height = 13,
    .value = NO
};

//약물 장전중
static uint8_t LOADING_DRUG[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0xc2, 0x3f, 0xe0,
    0x2f, 0xfe, 0xc2, 0x3f, 0x42, 0x4e, 0x20, 0x00, 0x21, 0x10, 0x02, 0x06,
    0x42, 0xc2, 0x3f, 0x00, 0x21, 0x90, 0x03, 0x09, 0x42, 0x02, 0x00, 0x80,
    0xe2, 0x28, 0xc2, 0x30, 0x3c, 0xee, 0x7f, 0x40, 0x24, 0x48, 0x02, 0x00,
    0x00, 0x02, 0x04, 0x20, 0x28, 0x84, 0xe2, 0x7f, 0x00, 0xc0, 0x3f, 0x00,
    0x00, 0x02, 0x02, 0x04, 0xfc, 0x03, 0x20, 0x80, 0x1f, 0x00, 0x82, 0x1f,
    0x00, 0xc2, 0x3f, 0x40, 0x20, 0x04, 0x40, 0x20, 0x00, 0x42, 0x00, 0x40,
    0x20, 0x04, 0x40, 0x20, 0x00, 0xc2, 0x3f, 0x80, 0x1f, 0xfc, 0x83, 0x1f,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t LOADING_DRUG_XBM = {
    .width = 64,
    .height = 13,
    .value = LOADING_DRUG
};

//약물/음수 비율
static uint8_t DRUG_WATER_SCALE[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
    0xc2, 0x3f, 0x20, 0x7e, 0x00, 0x01, 0x08, 0x09, 0x7e, 0x00, 0x42, 0x4e,
    0x20, 0x20, 0x81, 0x00, 0x01, 0x08, 0x09, 0x81, 0x00, 0x42, 0xc2, 0x3f,
    0x10, 0x81, 0x80, 0x02, 0x08, 0x09, 0x7e, 0x00, 0x42, 0x02, 0x00, 0x10,
    0x7e, 0x40, 0x04, 0x08, 0x09, 0x00, 0x00, 0x3c, 0xee, 0x7f, 0x08, 0x00,
    0x30, 0x18, 0xf8, 0x89, 0xff, 0x01, 0x00, 0x02, 0x04, 0x88, 0xff, 0x01,
    0x00, 0x08, 0x09, 0x42, 0x00, 0x00, 0xc0, 0x3f, 0x04, 0x00, 0x00, 0x00,
    0x08, 0x09, 0xff, 0x00, 0xfc, 0x03, 0x20, 0x04, 0xff, 0xf8, 0x1f, 0x08,
    0x09, 0x80, 0x00, 0x00, 0xc2, 0x3f, 0x02, 0x81, 0x00, 0x01, 0x08, 0x09,
    0xff, 0x00, 0x00, 0x42, 0x00, 0x02, 0x81, 0x00, 0x01, 0xf8, 0x09, 0x01,
    0x00, 0x00, 0xc2, 0x3f, 0x01, 0xff, 0x00, 0x01, 0x00, 0x08, 0xff, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

xbm_t DRUG_WATER_SCALE_XBM = {
    .width = 82,
    .height = 13,
    .value = DRUG_WATER_SCALE
};

//예
static uint8_t YES[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x8c, 0x02, 0x08, 0x80, 0x00, 0x92, 0x02,
    0x04, 0x02, 0x01, 0xf2, 0x02, 0x04, 0x07, 0x01, 0x92, 0x02, 0x82, 0x0a,
    0x02, 0x92, 0x02, 0x02, 0x02, 0x02, 0x92, 0x02, 0x02, 0x02, 0x02, 0xf2,
    0x02, 0x02, 0x02, 0x02, 0x92, 0x02, 0x02, 0x02, 0x02, 0x92, 0x02, 0x04,
    0x02, 0x01, 0x8c, 0x02, 0x04, 0x02, 0x01, 0x80, 0x02, 0x08, 0x82, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t YES_XBM = {
    .width = 35,
    .height = 13,
    .value = YES
};

//유속
static uint8_t WATER_SPEED[] = {
    0x00, 0x00, 0x00, 0xf8, 0x01, 0x04, 0x04, 0x02, 0x04, 0x04, 0x02, 0x1b,
    0x04, 0xc2, 0x60, 0xf8, 0x01, 0x04, 0x00, 0xe0, 0x7f, 0x00, 0x00, 0x00,
    0xfe, 0xc7, 0x3f, 0x08, 0x01, 0x20, 0x08, 0x01, 0x20, 0x08, 0x01, 0x20,
    0x00, 0x00, 0x00
};

xbm_t WATER_SPEED_XBM = {
    .width = 24,
    .height = 13,
    .value = WATER_SPEED
};

//음수량만 측정
static uint8_t MEASURE_ONLY_WATER[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x01,
    0x04, 0x7e, 0xe2, 0x27, 0x00, 0x0f, 0x7e, 0x02, 0x04, 0x02, 0x04, 0x40,
    0x2e, 0x24, 0x00, 0x00, 0x08, 0x02, 0x04, 0x02, 0x0a, 0x7e, 0x22, 0x24,
    0xc0, 0x3f, 0xc8, 0x03, 0xf8, 0x01, 0x11, 0x02, 0x22, 0xe4, 0x00, 0x06,
    0x18, 0x02, 0x00, 0xc0, 0x60, 0x02, 0x2e, 0x24, 0xc0, 0x39, 0x24, 0x02,
    0xfe, 0x07, 0x00, 0xfe, 0xe2, 0x27, 0x00, 0x00, 0x42, 0x02, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x20, 0xe0, 0x7f, 0x00, 0x00, 0xfc, 0xe3, 0x7f, 0xf8,
    0x01, 0x20, 0x00, 0x00, 0xf8, 0x01, 0x04, 0x02, 0x04, 0x04, 0x42, 0x00,
    0xc0, 0x3f, 0x04, 0x02, 0x04, 0x02, 0x04, 0x04, 0x42, 0x00, 0x00, 0x20,
    0x04, 0x02, 0xfc, 0x03, 0x04, 0xf8, 0xc1, 0x3f, 0x00, 0x20, 0xf8, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

xbm_t MEASURE_ONLY_WATER_XBM = {
    .width = 76,
    .height = 13,
    .value = MEASURE_ONLY_WATER
};

//정지
static uint8_t STOP[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0xe2, 0x2f, 0x80, 0x00, 0x08,
    0x08, 0x02, 0x21, 0x40, 0x20, 0x10, 0xc8, 0x03, 0x21, 0x40, 0x70, 0x10,
    0x18, 0x02, 0x21, 0x20, 0xa8, 0x20, 0x24, 0x02, 0x21, 0x20, 0x20, 0x20,
    0x42, 0x82, 0x22, 0x20, 0x20, 0x20, 0x00, 0x80, 0x22, 0x20, 0x20, 0x20,
    0xf8, 0x41, 0x24, 0x20, 0x20, 0x20, 0x04, 0x42, 0x24, 0x40, 0x20, 0x10,
    0x04, 0x22, 0x28, 0x40, 0x20, 0x10, 0xf8, 0x01, 0x20, 0x80, 0x20, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t STOP_XBM = {
    .width = 47,
    .height = 13,
    .value = STOP
};

//튜브 점검
static uint8_t CHECK_TUBE[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
    0xe2, 0x2f, 0x38, 0xe2, 0x27, 0x80, 0xf8, 0x87, 0x7f, 0x04, 0x42, 0x02,
    0x21, 0x00, 0x22, 0x3c, 0x40, 0x08, 0x84, 0x00, 0x08, 0x42, 0x02, 0x39,
    0xfe, 0x22, 0x24, 0x40, 0x08, 0x84, 0x00, 0x08, 0x42, 0x82, 0x22, 0x00,
    0x22, 0x24, 0x20, 0x08, 0x84, 0x00, 0x10, 0x42, 0x82, 0x24, 0x38, 0x22,
    0x3c, 0x20, 0x08, 0x84, 0x00, 0x10, 0x42, 0x42, 0x28, 0x44, 0xee, 0x27,
    0x20, 0xf8, 0x87, 0x7f, 0x10, 0x42, 0x22, 0x20, 0x44, 0x02, 0x20, 0x20,
    0x80, 0x00, 0x00, 0x10, 0x42, 0x02, 0x20, 0x38, 0x02, 0x20, 0x20, 0x80,
    0x00, 0x00, 0x10, 0x42, 0x42, 0x00, 0x10, 0x42, 0x00, 0x40, 0x80, 0x00,
    0x00, 0x08, 0x3c, 0x42, 0x00, 0xfe, 0x42, 0x00, 0x40, 0xfc, 0xcf, 0xff,
    0x08, 0x00, 0xc2, 0x3f, 0x00, 0xc2, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

xbm_t CHECK_TUBE_XBM = {
    .width = 86,
    .height = 13,
    .value = CHECK_TUBE
};

//이전화면 (모드)
static uint8_t BACK_PAGE_MODE[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c,
    0xe2, 0x2f, 0x38, 0xe2, 0x27, 0x80, 0xf8, 0x87, 0x7f, 0x04, 0x42, 0x02,
    0x21, 0x00, 0x22, 0x3c, 0x40, 0x08, 0x84, 0x00, 0x08, 0x42, 0x02, 0x39,
    0xfe, 0x22, 0x24, 0x40, 0x08, 0x84, 0x00, 0x08, 0x42, 0x82, 0x22, 0x00,
    0x22, 0x24, 0x20, 0x08, 0x84, 0x00, 0x10, 0x42, 0x82, 0x24, 0x38, 0x22,
    0x3c, 0x20, 0x08, 0x84, 0x00, 0x10, 0x42, 0x42, 0x28, 0x44, 0xee, 0x27,
    0x20, 0xf8, 0x87, 0x7f, 0x10, 0x42, 0x22, 0x20, 0x44, 0x02, 0x20, 0x20,
    0x80, 0x00, 0x00, 0x10, 0x42, 0x02, 0x20, 0x38, 0x02, 0x20, 0x20, 0x80,
    0x00, 0x00, 0x10, 0x42, 0x42, 0x00, 0x10, 0x42, 0x00, 0x40, 0x80, 0x00,
    0x00, 0x08, 0x3c, 0x42, 0x00, 0xfe, 0x42, 0x00, 0x40, 0xfc, 0xcf, 0xff,
    0x08, 0x00, 0xc2, 0x3f, 0x00, 0xc2, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
};

xbm_t BACK_PAGE_MODE_XBM = {
    .width = 86,
    .height = 13,
    .value = BACK_PAGE_MODE
};

#endif