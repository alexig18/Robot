/*
Possibly useful for finding can, interpreting the location, and navigating to it
*/


#ifndef CanNavigation_h
#define CanNavigation_h

#include "Arduino.h"
#include "ServoArm.h"

class CanNavi
{
    public:
    CanNavi();
    boolean findItem(); // to locate something within sonar field of robot
    boolean isCan(); // to confirm whether something is a can or not
    void goToCan(); // navigates to can
    void collectCan(); // control can pick up
    private:
    // the underscore helps show the variable is private
    boolean _item; // true if sonar detects an item within its range
    boolean _isCan; // true if isCan confirms the item found is a can
};

#endif