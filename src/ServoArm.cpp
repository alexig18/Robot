#include <ServoArm.h>

#define closedR 160
#define closedL 30
#define Lopen 180
#define Ropen 0

Serv _servoR;
Serv _servoL;

//default constructor
ServoArm::ServoArm()
{}

//perform setup
void ServoArm::begin(PinName pinLeft, PinName pinRight)
{
    
    _servoL.attach(pinLeft);
    _servoR.attach(pinRight);
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

void ServoArm::open() {
    this->ServoArm::openR();
    this->ServoArm::openL();
}

void ServoArm::close() {
    this->ServoArm::closeR();
    this->ServoArm::closeL();
}

void ServoArm::bump(){
    _servoL.write(closedL-30);
    _servoR.write(closedR+40);
    delay(500);
    this->ServoArm::close();
}

