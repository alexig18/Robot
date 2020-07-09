#include "Arduino.h"
#include "CanNavigation.h"
#include "ServoArm.h"

CanNavigation::CanNavigation()
{
    _item = findItem();
    if(_item) {
        _isCan = isCan();
        if(_isCan) {
            collectCan();
        }
    }
}

boolean CanNavigation::findItem()
{
    // use sonar to find item within 8''
}

boolean CanNavigation::isCan()
{
    /*
    dist = sonar inpu

    */
}

void CanNavigation::goToCan()
{
    // use whatever method we want to get to the can
}

void CanNavigation::collectCan() 
{
    // servo control to pick up can
}