#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Arduino.h>
#include <MotorControl.h>
#include <LiftCan.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MOTOR_A PA_8
#define MOTOR_B PB_8
#define MOTOR_A PA_8
#define MOTOR_B PB_8

#define PMWFREQ 1000
#define R_SERVO PA7
#define L_SERVO PA6




LiftCan cl;

int count = 0;

void setup() {  
  cl.begin(L_SERVO, R_SERVO);


  //delay(1000);
}

void loop() {
 cl.lift();
 delay(2000);
 cl.lower();
 delay(2000);
}