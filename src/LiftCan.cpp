#include <LiftCan.h>

#define LLBOUND 180
#define RLBOUND 0

#define moveangle 141


int _count = 0;

LiftCan::LiftCan() {}


Servo lservo;
Servo rservo;

void LiftCan::begin(int pinleft, int pinright)
{
    rservo.attach(pinright);
    lservo.attach(pinleft);
    rservo.write(RLBOUND);
    lservo.write(LLBOUND);
}

// instructions to operate the four bar linkage/lift the can into upper storage
void LiftCan::lift(int speed)
{
   while(_count != moveangle){
   lservo.write(LLBOUND-_count);
   rservo.write(RLBOUND+_count);
   _count++;
   delay(speed);
 }
 _count = 0;
}

void LiftCan::lower(int speed)
{
    while(_count != moveangle){
   lservo.write(LLBOUND+_count-moveangle);
   rservo.write(RLBOUND-_count+moveangle);
   _count++;
   delay(speed);    
    }
    _count = 0;
}