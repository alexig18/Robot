#include <ServoArm.h>

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
    _servoR.write(_armAngle);
}

void ServoArm::closeL()
{
    _servoL.write(-_armAngle);
}

void ServoArm::openR()
{
    _servoR.write(-_armAngle);
}

void ServoArm::openL()
{
    _servoL.write(_armAngle);
}

void ServoArm::openBoth() {
    openR();
    openL();
}

void ServoArm::closeBoth() {
    closeR();
    closeL();
}


