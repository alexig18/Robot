#include <ServoArm.h>

#define closedR 0
#define closedL 0
#define Lopen 0
#define Ropen 0

//default constructor
ServoArm::ServoArm()
{}

//perform setup
void ServoArm::begin(int pinRight, int pinLeft)
{
    _servoR.attach(pinRight);
    _servoL.attach(pinLeft);
    _servoR.write(closedR);
    _servoL.write(closedL);
    delay(500);
    _servoL.write(Lopen);
    _servoR.write(Ropen);

}

void ServoArm::closeR()
{
    _servoR.write(closedR);
}

void ServoArm::closeL()
{
    _servoL.write(closedL);
}

void ServoArm::openR()
{
    _servoR.write(Ropen);
}

void ServoArm::openL()
{
    _servoL.write(Lopen);
}

void ServoArm::openBoth() {
    this->ServoArm::openR();
    this->ServoArm::openL();
}

void ServoArm::closeBoth() {
    this->ServoArm::closeR();
    this->ServoArm::closeL();
}


