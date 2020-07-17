#include <LiftCan.h>

#define moveangle 141
#define speed 20

int _count = 0;

LiftCan::LiftCan() {}


Servo lservo;
Servo rservo;

void LiftCan::begin(int pinleft, int pinright)
{
    rservo.attach(pinright);
    lservo.attach(pinleft);
}

// instructions to operate the four bar linkage/lift the can into upper storage
void LiftCan::lift()
{
   while(_count != moveangle){
   lservo.write(180-_count);
   rservo.write(0+_count);
   _count++;
   delay(speed);
 }
 _count = 0;
}

void LiftCan::lower()
{
    while(_count != moveangle){
   lservo.write(180+_count-moveangle);
   rservo.write(0-_count+moveangle);
   _count++;
   delay(speed);    
    }
    _count = 0;
}