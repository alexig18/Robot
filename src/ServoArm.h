/*
Library to control servo arms
*/
#ifndef ServoArm_h
#define ServoArm_h

#include "Arduino.h"
#include "Servo.h"

class ServoArm
{
    public:
    ServoArm();
    void begin(int rightArmPin, int leftArmPin);
    void closeR();
    void closeL();
    void openR();
    void openL();
    void closeBoth();
    void openBoth();
    private:
    // the underscore helps show the variable is private
    Servo _servoR;
    Servo _servoL;
};

#endif