/*
Library to sense tape.
*/


#ifndef TestMode_h
#define TestMode_h

#include "Arduino.h"
#include "CanNavigation.h"
#include "ServoArm.h"
#include "MotorControl.h"
#include "ServoBackFlap.h"
#include "Serv.h"
#include "LiftCan.h"
#include "IR.h"
#include "TapeSense.h"
#include <Adafruit_SSD1306.h>


class TestMode
{
    public:
    TestMode();
    void testArms(ServoArm arm);
    void testFourBar(LiftCan cl);
    void testBackFlap(ServoBackFlap bf);
    void testMoveStraight(MotorControl motors);
    void testMoveBackwards(MotorControl motors);
    void testTurnLeft(MotorControl motors);
    void testTurnRight(MotorControl motors);  
    void testSonar(NewPing sonar, Adafruit_SSD1306 display);  
    void testDeposit(ServoArm arm, LiftCan cl);
};

#endif

ServoArm arms;

TestMode::TestMode(){}

//both arms close, delay 1 second, then open
void TestMode::testArms(ServoArm arm) {
    arm.close();
    delay(1000);
    arm.open();
} 

//test 4 bar movement up and down
void TestMode::testFourBar(LiftCan cl) {
   cl.lift(20);
   delay(1000);
   cl.lower(20);
   delay(1000);
}

//arms close, lifts up, bump, lifts down, arms open
void TestMode::testDeposit(ServoArm arm, LiftCan cl) {
   arm.close();
   delay(1000);
   cl.lift(20);
   delay(1000);
   arm.bump();
   delay(1000);
   cl.lower(20);
   delay(1000);
   arm.open();
   delay(1000);
}

void TestMode::testBackFlap(ServoBackFlap bf) {

}

//move straight for 4 seconds
void TestMode::testMoveStraight(MotorControl motors) {
    motors.move(400,400);
    delay(4000);
    motors.move(0,0);
}

void TestMode::testMoveBackwards(MotorControl motors) {
    motors.move(-400,-400);
    delay(4000);
    motors.move(0,0);
}

void TestMode::testTurnLeft(MotorControl motors) {
    motors.move(-400,400);
    delay(4000);
    motors.move(0,0);
}

void TestMode::testTurnRight(MotorControl motors) {
    motors.move(400,-400);
    delay(4000);
    motors.move(0,0);
}

//infinite loop so only function is to display the distance the sonar is reading 
void TestMode::testSonar(NewPing sonar, Adafruit_SSD1306 display) {
    while(true) {
        display.clearDisplay();
        display.println("Sonar reading: ");
        display.println(sonar.convert_cm(sonar.ping_median()));
        display.display();
        delay(500);
    }
}