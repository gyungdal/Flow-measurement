#include "motor.h"
#include <Arduino.h>

void Motor::begin(motor_t* motor){
    this->motor = motor;

    pinMode(this->motor->pwmPin, OUTPUT);
    analogWrite(this->motor->pwmPin, 0);

    for(uint8_t i = 0;i<2;i++){
        pinMode(this->motor->sigPin[i], INPUT_PULLUP);
    }
}

void Motor::run(){
    analogWrite(this->motor->pwmPin, 255);
}

void Motor::release(){
    analogWrite(this->motor->pwmPin, 0);
}

void Motor::firstTick(){
    this->motor->angle = 0;
    this->motor->lastTime = micros();
}

void Motor::secondTick(){
    this->motor->angle = 1;
    this->motor->lastTime = micros();
}
