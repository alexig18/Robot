/*
Library to help control motors
*/

#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"

class MotorControl
{
    public:
<<<<<<< HEAD
    MotorControl(int pin);
    void accel();
    void decel();
    void constV();
    private:
    // the underscore helps show the variable is private
    int _pin;
=======
    MotorControl();
    void begin (int lPinLM, int rPinLM, int lPinRM, int rPinRM, int freq);
    void goToAngle(String direction, int delay);
    void move(int leftmot, int rightmot);
    int leftFreq();
    int rightFreq();
    
    private:
    // the underscore helps show the variable is private
    PinName _fOutLM;
    PinName _bOutLM;
    PinName _fOutRM;
    PinName _bOutRM;

    int _frequency;
    int _leftFreq;
    int _rightFreq;
>>>>>>> 78d9479ccf28545718c3b87a08429f041f14e019
};

#endif
