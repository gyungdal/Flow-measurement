#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <PWM.h>
#include "type.h"

class Motor {
    public:
        void begin(motor_t*);
    private:
        motor_t* motor;
        void firstTick();
        void secondTick();
        void thirdTick();
}; 

#endif