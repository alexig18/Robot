#include "Arduino.h"
#include "MotorControl.h"

<<<<<<< HEAD
MotorControl::MotorControl(int pinOut)
{
    pinMode(pinOut, OUTPUT);
    _pin = pinOut;
}

void MotorControl::accel()
{                           
// instructions to speed up
=======
MotorControl::MotorControl(){}

void MotorControl::begin(int lPinLM, int rPinLM, int lPinRM, int rPinRM, int freq)
{
    pinMode(lPinLM, OUTPUT);
    pinMode(rPinLM, OUTPUT);
    pinMode(lPinRM, OUTPUT);
    pinMode(rPinRM, OUTPUT);

    _fOutLM = PinName(lPinLM);
    _bOutLM = PinName(rPinLM);
    _fOutRM = PinName(rPinRM);
    _bOutRM = PinName(lPinRM);

    _frequency = freq;
>>>>>>> 78d9479ccf28545718c3b87a08429f041f14e019
}

void MotorControl::decel()
{
<<<<<<< HEAD
    // instructions to slow down
=======
  _leftFreq = leftmot;
  _rightFreq = rightmot;
  if (leftmot > 0 && rightmot > 0) // both forward
  {
    pwm_start(_fOutLM, _frequency, leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, rightmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
  }
  else if (leftmot < 0 && rightmot < 0) // both backward
  {
    pwm_start(_fOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, -leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, -rightmot, RESOLUTION_10B_COMPARE_FORMAT); 
  }
  else if (leftmot == 0 && rightmot == 0) // stopped
  {
    pwm_start(_fOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT); 
  }
  else if (leftmot > 0 && rightmot < 0) // turn right
  {
    pwm_start(_fOutLM, _frequency, leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, -rightmot, RESOLUTION_10B_COMPARE_FORMAT);  
  }
  else
  {
    pwm_start(_fOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, -leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, rightmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT); 
  }
>>>>>>> 78d9479ccf28545718c3b87a08429f041f14e019
}

void MotorControl::constV()
{
<<<<<<< HEAD
    // keep speed constant
    // not sure if this is necessary
=======
    if (direction == "L") //left
    {
        move(-500, 500);
        delay(wait);
    }
    else if (direction == "R") //right
    {
        move(500, -500);
        delay(wait);
    }
}

int MotorControl::leftFreq()
{
  return _leftFreq;
}

int MotorControl::rightFreq()
{
  return _rightFreq;
>>>>>>> 78d9479ccf28545718c3b87a08429f041f14e019
}