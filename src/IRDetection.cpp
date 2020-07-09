#include <IRDetection.h>

IRDetection::IRDetection(int pinOut)
{
    pinMode(pinOut, INPUT);
    // do we want a threshold value for what the signal has to be above?
    _pin = pinOut;
}

void IRDetection::locateBin()
{
// instructions to locate the recycling bin
}

void IRDetection::moveToBin()
{
    // instructions to navigate to the recycling bin
}

void IRDetection::atBin()
{
    // instructions of what to do once at recylcing bin
}

