#include <IR.h>

IR::IR(){}

void IR::begin(int pinIn, MotorControl lMotor, MotorControl rMotor)
{
    pinMode(pinIn, INPUT);
    // do we want a threshold value for what the signal has to be above?
    _pIn = pinIn;
    pinMode(_pIn, INPUT);
}

void IR::locateBin()
{

// instructions to locate the recycling bin
}

void IR::moveToBin()
{
    // instructions to navigate to the recycling bin
}

void IR::atBin()
{
    // instructions of what to do once at recylcing bin
}

