#ifndef CanNavigation_h
#define CanNavigation_h

#include "Arduino.h"
#include "ServoArm.h"

class CanNavigation
{
    public:
    CanNavigation();
    bool findItem(); 
    // to locate something within sonar field of robot
    bool isCan(); 
    // to confirm whether something is a can or not
    void goToCan(); 
    // navigates to can
    void collectCan(); 
    // control can pick up
    private:
    // the underscore helps show the variable is private
    int _sPin;
    bool _item; // true if sonar detects an item within its range
    bool _isCan; // true if isCan confirms the item found is a can
};

#endif