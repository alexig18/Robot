/*
Library to control servo arms
*/
#ifndef ServoArm_h
#define ServoArm_h

#include "Arduino.h"
#include "Serv.h"

class ServoArm
{
    public:
    ServoArm();
    void begin(PinName rightArmPin, PinName leftArmPin);
    void closeR();
    void closeL();
    void openR();
    void openL();
    void close();
    void open();
    void bump();
    void setAngle(int angle);
    private:
    // the underscore helps show the variable is private
    Serv _servoR;
    Serv _servoL;
    int _armAngle = 160;
};

#endif