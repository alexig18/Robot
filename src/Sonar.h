/*
Q1: How to tell can from bin/wall
A1: Do some trig with distance to figure out how much robot should rotate to either side before can sensing disappears -- turn robot that much in each direction and if it’s still sensing something, not a can
If too far away maybe move toward ‘can’ a certain amount and do again
Concern: time, accuracy

*/


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