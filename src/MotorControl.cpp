#include "Arduino.h"
#include "MotorControl.h"


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

void MotorControl::decel()
{

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

}

void MotorControl::constV()
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

int MotorControl::leftFreq()
{
  return _leftFreq;
}

int MotorControl::rightFreq()
{
  return _rightFreq;

}