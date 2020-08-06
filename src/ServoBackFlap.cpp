#include <ServoBackFlap.h>
#define openAng 180
#define closedAng 80

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
