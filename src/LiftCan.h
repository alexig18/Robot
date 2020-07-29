/*
Library to control the four-bar linkage to lift the can into upper storage
*/
#ifndef LiftCan_h
#define LiftCan_h

#include "Arduino.h"
#include "Serv.h"

class LiftCan
{
    public:
    LiftCan();
    void begin(PinName rightBarPin, PinName leftBarPin);
    void lift(int speed);
    void lower(int speed);
    private:
};

#endif