/*
Library to sense tape.
*/


#ifndef TapeSense_h
#define TapeSense_h

#include "Arduino.h"

class TapeSense
{
    public:
    TapeSense(int pinIn);
    void read();
    private:
    // the underscore helps show the variable is private
    int _pIn;
    int _tape;
};

#endif