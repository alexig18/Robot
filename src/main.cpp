#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Arduino.h>
#include <MotorControl.h>
#include <LiftCan.h>
#include<ServoArm.h>
#include<ServoBackFlap.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MOTOR_L_R PB_6
#define MOTOR_L_L PB_8
#define MOTOR_R_L PB_7
#define MOTOR_R_R PB_9

#define PMWFREQ 1000
#define L4 PB1
#define R4 PB0
#define LA PA7
#define RA PA6
#define BACK PA3

LiftCan cl;
ServoArm arm;
ServoBackFlap back;
MotorControl motors;

int count = 0;
void canDeposit(){
  arm.close();
  delay(1000);
  cl.lift(20);
  delay(1000);
  arm.bump();
  delay(1000);
  cl.lower(20);
  delay(1000);
  arm.open();
}

void setup() {  
  cl.begin(L4, R4);
  arm.begin(LA, RA);
  back.begin(BACK);
  motors.begin(MOTOR_L_L, MOTOR_L_R, MOTOR_R_L, MOTOR_R_R, PMWFREQ);

delay(2000);
}

void loop() {
delay(5000);
canDeposit();
delay(5000);

//arm.close();
//delay(10000);
//arm.bump();
}

