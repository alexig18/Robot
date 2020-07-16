/*
Library to control the detection and navigation to the recycling bin
*/
#ifndef IR_h
#include <MotorControl.h>
#define IR_h

#include "Arduino.h"

class IR
{
    public:
    IR();
    void begin(int pinIn, MotorControl lMotor, MotorControl rMotor);
    void locateBin();
    void moveToBin();
    void atBin();
    private:
    // the underscore helps show the variable is private
    int _pIn;
    MotorControl _lM;
    MotorControl _rM;
};

#endif