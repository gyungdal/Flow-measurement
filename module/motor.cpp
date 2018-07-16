#include "motor.h"
#include <Arduino.h>

static const int maxTick = 3242U;
void Motor::begin(motor_t* motor){
    this->motor = motor;

    pinMode(this->motor->pwmPin, OUTPUT);
    release();

    for(uint8_t i = 0;i<2;i++){
        pinMode(this->motor->sigPin[i], INPUT_PULLUP);
    }
}

void Motor::run(uint32_t value){
    digitalWrite(this->motor->pwmPin, LOW);
}

void Motor::release(){
    digitalWrite(this->motor->pwmPin, HIGH);
}

void Motor::firstTick(){
    this->motor->angle = 0;
    this->motor->lastTime = micros();
}

void Motor::secondTick(){
    this->motor->angle = 1;
    this->motor->lastTime = micros();
}
