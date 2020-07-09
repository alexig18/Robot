#include "Arduino.h"
#include "MotorControl.h"
#include "ctime"

MotorControl::MotorControl(int lPinLM, int rPinLM, int lPinRM, int rPinRM, int freq)
{
    pinMode(lPinLM, OUTPUT);
    pinMode(rPinLM, OUTPUT);
    pinMode(lPinRM, OUTPUT);
    pinMode(rPinRM, OUTPUT);

    lPinLM = _fOutLM;
    rPinLM = _bOutLM;
    rPinRM = _fOutRM;
    lPinRM = _bOutRM;

    _frequency = freq;
}

void MotorControl::addSpeed()
{                           
   // increments dtyCycle each time it is called

}

void MotorControl::decrementSpeed()
{
    // decrements dtyCycle each time it is called
}

void MotorControl::foreward(int dytCycleF)
{
    // instantly defines new forewards speed from 0 to 100%
    pwm_start(_fOutLM, _frequency, dytCycleF, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, dytCycleF, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
}

void MotorControl::backwards(int dytCycleB)
{
    // instantly defines new backwards speed from 0 to 100%
    pwm_start(_fOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, dytCycleB, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, dytCycleB, RESOLUTION_10B_COMPARE_FORMAT);
}

void MotorControl::goToAngle(int delay)
{
    
}