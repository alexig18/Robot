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

void MotorControl::move(int leftmot, int rightmot)
{
  if (leftmot > 0 && rightmot > 0)
  {
    pwm_start(_fOutLM, _frequency, leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, rightmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
  }
  else if (leftmot < 0 && rightmot < 0)
  {
    pwm_start(_fOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, rightmot, RESOLUTION_10B_COMPARE_FORMAT); 
  }
  else if (leftmot == 0 && rightmot == 0)
  {
    pwm_start(_fOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT); 
  }
  else if (leftmot > 0 && rightmot < 0)
  {
    pwm_start(_fOutLM, _frequency, leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, rightmot, RESOLUTION_10B_COMPARE_FORMAT);  
  }
  else
  {
    pwm_start(_fOutLM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutLM, _frequency, leftmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_fOutRM, _frequency, rightmot, RESOLUTION_10B_COMPARE_FORMAT);
    pwm_start(_bOutRM, _frequency, 0, RESOLUTION_10B_COMPARE_FORMAT); 
  }
}

void MotorControl::goToAngle(String direction, int wait)
{
    if (direction == "L") //left
    {
        move(-50, 50);
        delay(wait);
    }
    else if (direction == "R") //right
    {
        move(50, -50);
        delay(wait);
    }
}