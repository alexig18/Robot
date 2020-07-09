#include "Arduino.h"
#include "Sonar.h"

#define SAMPLES 10



Sonar::Sonar(int trigger, int echo, int maxDistance, int offset)
{
    _trigger = trigger;
    _echo = echo;
    _maxDistance = maxDistance;
    _offset = offset;
    _newPing = NewPing(trigger,echo,maxDistance);
 
}



// returns distance measured by Sonar
int Sonar::dist()
{

    return  _newPing.ping_median(SAMPLES) + _offset;

}

