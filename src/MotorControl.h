/*
Library to help control motors
*/

#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"

class MotorControl
{
    public:
    MotorControl(int pin);
    void accel();
    void decel();
    void constV();
    private:
    // the underscore helps show the variable is private
    int _pin;
};

#endif
