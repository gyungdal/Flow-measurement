#include "motor.h"

void Motor::begin(motor_t* motor){
    this->motor = motor;

    pinMode(this->motor->pwmPin, OUTPUT);
    analogWrite(this->motor->pwmPin, 0);

    for(uint8_t i = 0;i<2;i++){
        pinMode(this->motor->sigPin[i], INPUT_PULLUP);
    }

    attachInterrupt(digitalPinToInterrupt(motor->sigPin[0]), firstTick, RISING);
    attachInterrupt(digitalPinToInterrupt(motor->sigPin[1]), secondTick, RISING);
}

void Motor::firstTick(){
    this->motor->angle = 0;
    this->motor->lastTime = micros();
}

void Motor::secondTick(){
    this->motor->angle = 1;
    this->motor->lastTime = micros();
}
