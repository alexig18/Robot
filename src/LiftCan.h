/*
Library to control the four-bar linkage to lift the can into upper storage
*/
#ifndef LiftCan_h
#define LiftCan_h

#include "Arduino.h"
#include "Servo.h"

class LiftCan
{
    public:
    LiftCan();
    void begin(int rightBarPin, int leftBarPin);
    void lift();
    void lower();
    private:
    // the underscore helps show the variable is private
    Servo _servoR;
    Servo _servoL;
};

#endif