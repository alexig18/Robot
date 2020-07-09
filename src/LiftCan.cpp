#include <LiftCan.h>

Servo servoR;
Servo servoL;

LiftCan::LiftCan(int pinRight, int pinLeft)
{
    servoR.write(0);
    servoL.write(0);
    servoR.attach(pinRight);
    servoL.attach(pinLeft);
    _pinR = pinRight;
    _pinL = pinLeft;
}

// instructions to operate the four bar linkage/lift the can into upper storage
void LiftCan::lift()
{
    servoR.write(-160);
    servoL.write(160);
    //Amount of time before the lift moves back down
    delay(5000);
    servoR.write(160);
    servoL.write(-160);
}


