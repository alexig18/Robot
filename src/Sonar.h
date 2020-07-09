#ifndef Sonar_h
#define Sonar_h

#include "Arduino.h"
#include "NewPing.h"

class Sonar
{
    public:
    Sonar (int trigger, int echo, int maxDistance, int offset);
    int dist();
    private:
    // the underscore helps show the variable is private
    int _trigger;
    int _echo;
    int _maxDistance;
    int _offset;
    NewPing _newPing; 
};

#endif