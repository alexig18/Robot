#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Arduino.h>
#include <MotorControl.h>
#include <LiftCan.h>

#define SERVO PA0
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define INPUT PA7
#define POT_SENSOR PA6
#define PROG_CONTROL PB5
#define MOTOR_A PA_8
#define MOTOR_B PB_8
#define PMWFREQ 512
#define R_SERVO PB5
#define L_SERVO PA5

Servo servo;
LiftCan cL = LiftCan();

void setup() {  
  cL.begin(R_SERVO, L_SERVO);
}

void loop() {
  
  cL.lift();
  delay(5000);
  cL.lower();
} 
