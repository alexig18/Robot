/*
Library to control servo arms
*/
#ifndef ServoArm_h
#define ServoArm_h

#include "Arduino.h"

class ServoArm
{
    public:
    ServoArm(int leftArmPin, int rightArmPin);
    void closeR();
    void closeL();
    void openR();
    void openL();
    void closeBoth();
    void openBoth();
    private:
    // the underscore helps show the variable is private
    int _pinL;
    int _pinR;
};

#endif