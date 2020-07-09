/*
Library to control the detection and navigation to the recycling bin
*/
#ifndef IRDetection_h
#define IRDetection_h

#include "Arduino.h"

class IRDetection
{
    public:
    IRDetection(int pin);
    void locateBin();
    void moveToBin();
    void atBin();
    private:
    // the underscore helps show the variable is private
    int _pin;
};

#endif