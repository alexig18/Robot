#include <ServoBackFlap.h>
#define BACKANG 90

Servo servo;

// defauly constructor
ServoBackFlap::ServoBackFlap()
{}

//initialise

void ServoBackFlap::begin(int pin)
{
    servo.write(0);
    servo.attach(pin);
    
}

void ServoBackFlap::open()
{
// instructions to open the upper storage bin backflap, releasing the cans
    servo.write(BACKANG);
}

void ServoBackFlap::close()
{
    // instructions to close the upper storage bin backflap
    servo.write(-BACKANG);
}
