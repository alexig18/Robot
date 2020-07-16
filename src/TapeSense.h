/*
Library to sense tape.
*/


#ifndef TapeSense_h
#define TapeSense_h

#include "Arduino.h"

class TapeSense
{
    public:
    TapeSense();
    void begin(int pinIn);
    bool read(); // returns true if tape is sensed
    private:
    int _pIn;
    int _readVal;
    int _threshold = 800; // threshold for if tape is sensed
};

#endif