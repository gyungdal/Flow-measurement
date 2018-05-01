#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "type.h"

class Motor {
    public:
        void begin(motor_t*);
        void run();
        void release();
        void firstTick();
        void secondTick();
    private:
        motor_t* motor;
}; 

#endif