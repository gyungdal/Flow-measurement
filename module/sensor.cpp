#include "sensor.h"

void Sensor::begin(sensor_t* sensor){
    this->sensor = sensor;
    pinMode(this->sensor->pin, INPUT);
    attachInterrupt(this->sensor->pin, this->tickHandler, RISING);
}

void Sensor::tickHandler(){
    this->sensor++;
}