/*
Library to help control motors
*/

#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"

class MotorControl
{
    public:
    MotorControl();
    void begin (int lPinLM, int rPinLM, int lPinRM, int rPinRM, int freq);
    void goToAngle(String direction, int delay);
    void move(int leftmot, int rightmot);
    
    private:
    // the underscore helps show the variable is private
    PinName _fOutLM;
    PinName _bOutLM;
    PinName _fOutRM;
    PinName _bOutRM;

    int _frequency;
};

#endif
