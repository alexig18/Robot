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
    LiftCan(int rightBarPin, int leftBarPin);
    void lift();
    //do we need a lower function too?
    private:
    // the underscore helps show the variable is private
    int _pinL;
    int _pinR;
};

#endif