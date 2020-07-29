/*
PWM servo library
*/
#ifndef Serv_h
#define Serv_h

#include "Arduino.h"

class Serv
{
    public:
    Serv();
    void attach(PinName servpin);
    void write(int val);
    private:
    PinName _pinS;
};

<<<<<<< HEAD
#endif
=======
#endif
>>>>>>> 31f66aa5499a8f170976daa4939d9550ff8f0b5f
