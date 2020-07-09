#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Arduino.h>
#include <MotorControl.h>
#include <Sonar.h>

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

Servo servo;

void setup() {  
  Serial.begin(9600);
  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(SSD1306_WHITE); 
  // display.setCursor(0,0); 

  // pinMode(MOTOR_A, OUTPUT);
  // pinMode(MOTOR_B, OUTPUT);

  servo.attach(SERVO);

}

void loop() {
  servo.write(0);
  delay(2000);

  servo.write(45);
  delay(2000);

  servo.write(90);
  delay(2000);
} 
