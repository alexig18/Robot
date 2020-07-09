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
    int _offset;
    // the underscore helps show the variable is private
    NewPing* _newPing; 
};

#endif