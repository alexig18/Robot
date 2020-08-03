#include "Arduino.h"
#include "MotorControl.h"
#include "ctime"

PinName _fOutLM;
PinName _bOutLM;
PinName _fOutRM;
PinName _bOutRM;
int _leftSpeed;
int _rightSpeed;

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
}

void MotorControl::move(int leftmot, int rightmot)
{
  _leftSpeed = leftmot;
  _rightSpeed = rightmot;

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
    pwm_start(_fOutLM, 0, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, 0, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, 0, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, 0, 0, RESOLUTION_10B_COMPARE_FORMAT); 
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
}

int MotorControl::leftSpeed() {
  return _leftSpeed;
}

int MotorControl::rightSpeed() {
  return _rightSpeed;
}

void MotorControl::goToAngle(String direction, int wait)
{
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