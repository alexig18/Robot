#include <ServoBackFlap.h>
#define openAng 0
#define closedAng 125

Serv servo;

// defauly constructor
ServoBackFlap::ServoBackFlap()
{}

//initialise

void ServoBackFlap::begin(PinName pin)
{
    servo.attach(pin);
    servo.write(closedAng);
}

void ServoBackFlap::open()
{
// instructions to open the upper storage bin backflap, releasing the cans
    servo.write(openAng);
}

void ServoBackFlap::close()
{
    // instructions to close the upper storage bin backflap
    servo.write(closedAng);
}
