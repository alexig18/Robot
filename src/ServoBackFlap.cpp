#include <ServoBackFlap.h>

Servo servo;

ServoBackFlap::ServoBackFlap(int pinOut)
{
    servo.write(0);
    servo.attach(pinOut);
    _pin = pinOut;
}

void ServoBackFlap::open()
{
// instructions to open the upper storage bin backflap, releasing the cans
}

void ServoBackFlap::close()
{
    // instructions to close the upper storage bin backflap
}
