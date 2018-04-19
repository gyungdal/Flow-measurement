#ifndef __SENSOR_H__
#define __SENSOR_H__

#include "type.h"

class Sensor{
    public:
        void begin(sensor_t*);
    private:
        sensor_t* sensor;
        void tickHandler();
};

#endif