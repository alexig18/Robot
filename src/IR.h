/*
Library to manage IR detection and navigation
*/
#ifndef IR_h
#define IR_h

#include "Arduino.h"

class IR
{
    public:
    IR(int pIn); // not sure what to take as input
    boolean findBeacon(); // look around for IR beacon and change boolean beacon once found
    void goTo(); // navigate to beacon
    private:
    // the underscore helps show the variable is private
    int _pIn;
    boolean beacon;
};

#endif