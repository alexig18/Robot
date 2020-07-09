#include <ServoArm.h>
#define ARM_ANGLE 160

//default constructor
ServoArm::ServoArm()
{}

//perform setup
void ServoArm::begin(int pinRight, int pinLeft)
{
    _servoR.write(0);
    _servoL.write(0);
    _servoR.attach(pinRight);
    _servoL.attach(pinLeft);
}

void ServoArm::closeR()
{
    _servoR.write(ARM_ANGLE);
}

void ServoArm::closeL()
{
    _servoL.write(-ARM_ANGLE);
}

void ServoArm::openR()
{
    _servoR.write(-ARM_ANGLE);
}

void ServoArm::openL()
{
    _servoL.write(ARM_ANGLE);
}

void ServoArm::openBoth() {
    openR();
    openL();
}

void ServoArm::closeBoth() {
    closeR();
    closeL();
}


