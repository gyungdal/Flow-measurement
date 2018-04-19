#include "motor.h"

void Motor::begin(motor_t* motor){
    this->motor = motor;

    pinMode(this->motor->pwmPin, OUTPUT);

    for(uint8_t i = 0;i<3;i++){
        pinMode(this->motor->pin[i], INPUT);
    }

    attachInterrupt(digitalPinToInterrupt(motor->pin[0]), firstTick, RISING);
    attachInterrupt(digitalPinToInterrupt(motor->pin[1]), secondTick, RISING);
    attachInterrupt(digitalPinToInterrupt(motor->pin[2]), thirdTick, RISING);
}

void Motor::firstTick(){
    this->motor->angle = 0;
    this->motor->lastTime = micros();
}

void Motor::secondTick(){
    this->motor->angle = 1;
    this->motor->lastTime = micros();
}

void Motor::thirdTick(){
    this->motor->angle = 2;
    this->motor->lastTime = micros();
}
