#include "Arduino.h"
#include "Sonar.h"

Sonar::Sonar(int pinIn)
{
    pinMode(pinIn, INPUT);
    _pin = pinIn;
}

// returns distance measured by Sonar
int Sonar::dist()
{

}