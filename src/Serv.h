/*
PWM servo library
*/
#ifndef Serv_h
#define Serv_h

#include "Arduino.h"

class Serv
{
    public:
    Serv();
    void attach(PinName servpin);
    void write(int val);
    private:
    PinName _pinS;
};

#endif
