/*
Library to control servo backflap of upper storage
*/
#ifndef ServoBackFlap_h
#define ServoBackFlap_h

#include "Arduino.h"
#include "Servo.h"

class ServoBackFlap
{
    public:
    ServoBackFlap();
    void begin(int pin);

    void open();
    void close();
    private:
    // the underscore helps show the variable is private
   
};

#endif