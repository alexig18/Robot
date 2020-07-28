#include "Arduino.h"
#include "CanNavigation.h"
#include "ServoArm.h"
#include "MotorControl.h"
#include <string>

#define DETECTION_DISTANCE 60 //in cm
#define ITERATIONS 5
#define TURNTIME 700 //ms

CanNavigation:: CanNavigation(int echo,int trig,int max,MotorControl motors)
{
    
    _sonar = new NewPing(trig,echo,max);
    _motors = motors;
}

int CanNavigation::Measure()
{
    int dist = _sonar->convert_cm(_sonar->ping_median(ITERATIONS));
    return dist;
}


bool CanNavigation::seek()
{
    _motors.move(400,-400);
    if (CanNavigation::pingDelay(TURNTIME, DETECTION_DISTANCE)) {
        return true;
    }
    _motors.move(-400,400);
    if (CanNavigation::pingDelay(2 * TURNTIME, DETECTION_DISTANCE)) {
        return true;
    }
    return false;
}
void  CanNavigation::move(String direction,int tim)
{

    if (direction.compareTo("left")){
        _motors.move(-400,400);

    }
    else if (direction.compareTo("right")){
        _motors.move(400,-400);
    }


}
bool  CanNavigation::pingDelay(int interval, int threshold){
    unsigned long  int currentTime = millis() , lastTime;
    while(currentTime - lastTime < interval){
        lastTime = millis();
        int dist = CanNavigation::Measure();
        if(dist <= threshold){
            _motors.move(0,0);
            //set global variable
            _distance = dist;
            return true;
        }
    }
    return false;
}
bool CanNavigation::findCan()
{

    //  Loops until can is found and then return true
    delete _sonar;
    return false;
}