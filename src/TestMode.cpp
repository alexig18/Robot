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
    TestMode(ServoArm arm, LiftCan cl, ServoBackFlap bf, MotorControl motors);
    void testArms();
    void testFourBar();
    void testBackFlap();
    void testMoveStraight();
    void testMoveBackwards();
    void testTurnLeft();
    void testTurnRight();  
    void testSonar(NewPing sonar, Adafruit_SSD1306 display);  
    void testDeposit();
    private: 
    ServoArm _arm;
    LiftCan _cl;
    ServoBackFlap _bf;
    MotorControl _motors;
};

#endif

ServoArm arms;

TestMode::TestMode(ServoArm arm, LiftCan cl, ServoBackFlap bf, MotorControl motors){
    _arm = arm;
    _cl = cl;
    _bf = bf;
    _motors = motors;
}

//both arms close, delay 1 second, then open
void TestMode::testArms() {
    _arm.close();
    delay(1000);
    _arm.open();
} 

//test 4 bar movement up and down
void TestMode::testFourBar() {
   _cl.lift(20);
   delay(1000);
   _cl.lower(20);
   delay(1000);
}

//arms close, lifts up, bump, lifts down, arms open
void TestMode::testDeposit() {
   _arm.close();
   delay(1000);
   _cl.lift(20);
   delay(1000);
   _arm.bump();
   delay(1000);
   _cl.lower(20);
   delay(1000);
   _arm.open();
   delay(1000);
}

void TestMode::testBackFlap() {

}

//move straight for 4 seconds
void TestMode::testMoveStraight() {
    _motors.move(400,400);
    delay(4000);
    _motors.move(0,0);
}

void TestMode::testMoveBackwards() {
    _motors.move(-400,-400);
    delay(4000);
    _motors.move(0,0);
}

void TestMode::testTurnLeft() {
    _motors.move(-400,400);
    delay(4000);
    _motors.move(0,0);
}

void TestMode::testTurnRight() {
    _motors.move(400,-400);
    delay(4000);
    _motors.move(0,0);
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