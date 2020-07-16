#include <TapeSense.h>

#define THRESHOLD 800

TapeSense::TapeSense(){}

void TapeSense::begin(int pinIn) {
    _pIn = pinIn;
    pinMode(_pIn, INPUT); // set pinIn as input pin
}

bool TapeSense::read() {
    _readVal = analogRead(_pIn);
    if (_readVal >= _threshold) { // determines whether value sensed corresponds to tape or not
        return true;
    } else {
        return false;
    }
}