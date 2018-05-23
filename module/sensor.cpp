#include "sensor.h"

void Sensor::begin(sensor_t* sensor){
    this->sensor = sensor;
    pinMode(this->sensor->pin, INPUT);
}

void Sensor::tickHandler(){
    this->sensor++;
    
}