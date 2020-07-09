#include "Arduino.h"
#include "MotorControl.h"

MotorControl::MotorControl(int pinOut)
{
    pinMode(pinOut, OUTPUT);
    _pin = pinOut;
}

void MotorControl::accel()
{
// instructions to speed up
}

void MotorControl::decel()
{
    // instructions to slow down
}

void MotorControl::constV()
{
    // keep speed constant
    // not sure if this is necessary
}