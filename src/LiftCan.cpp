#include <LiftCan.h>
#define LINKAGE_ANGLE 160


LiftCan::LiftCan() {}

void LiftCan::begin(int pinRight, int pinLeft)
{
    _servoR.write(0);
    _servoL.write(LINKAGE_ANGLE);
    _servoR.attach(pinRight);
    _servoL.attach(pinLeft);
}

// instructions to operate the four bar linkage/lift the can into upper storage
void LiftCan::lift()
{
    // for(int i = 0; i <= LINKAGE_ANGLE; i++) {
    //     _servoR.write(i);
    //     _servoL.write(LINKAGE_ANGLE-i);
    //     delay(50);
    // }
    _servoR.write(LINKAGE_ANGLE);
    _servoL.write(0);
}

void LiftCan::lower()
    {
    //     for(int i = LINKAGE_ANGLE; i >= 0; i--) {
    //     _servoR.write(i);
    //     _servoL.write(LINKAGE_ANGLE - i);
    //     delay(50);
    // }
    _servoR.write(0);
    _servoL.write(LINKAGE_ANGLE);
    }


