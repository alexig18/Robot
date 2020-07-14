#include <LiftCan.h>
#define LINKAGE_ANGLE 160
#define DELAY 5000


LiftCan::LiftCan() {}

void LiftCan::begin(int pinRight, int pinLeft)
{
    _servoR.write(0);
    _servoL.write(0);
    _servoR.attach(pinRight);
    _servoL.attach(pinLeft);
}

// instructions to operate the four bar linkage/lift the can into upper storage
void LiftCan::lift()
{
    _servoR.write(-LINKAGE_ANGLE);
    _servoL.write(LINKAGE_ANGLE);
}

void LiftCan::lower()
{
    _servoR.write(LINKAGE_ANGLE);
    _servoL.write(-LINKAGE_ANGLE);
}


