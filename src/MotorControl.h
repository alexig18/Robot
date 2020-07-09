/*
Library to help control motors
*/

#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"

class MotorControl
{
    public:
    MotorControl (int lPinLM, int rPinLM, int lPinRM, int rPinRM, int freq);
    void addSpeed();
    void decrementSpeed();
    void foreward(int dytCycleF);
    void backwards(int dytCycleB);
    void goToAngle(int delay);
    private:
    // the underscore helps show the variable is private
    PinName _fOutLM;
    PinName _bOutLM;
    PinName _fOutRM;
    PinName _bOutRM;

    int _frequency;
};

#endif
