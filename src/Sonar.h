#ifndef Sonar_h
#define Sonar_h

#include "Arduino.h"

class Sonar

    public:
    Sonar(int pinIn);
    void read();
    int dist();
    private:
    // the underscore helps show the variable is private
    int _pin;
};

#endif