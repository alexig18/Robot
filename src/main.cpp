#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Arduino.h>
#include <MotorControl.h>
#include <Sonar.h>
#include <ctime>

#define SERVO PA0
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define INPUT PA7
#define PROG_CONTROL PB5
#define MOTOR_L_R PA_8 // left motor right pin
#define MOTOR_L_L PB_8 // left motor left pin
#define MOTOR_R_R PA_3 // right motor forward
#define MOTOR_R_L PB_3 // right motor backward
#define PWMFREQ 512
#define cycles 10

Servo servo;
MotorControl motors;
int count;

void setup() {  
  motors = MotorControl(MOTOR_L_L, MOTOR_L_R, MOTOR_R_L, MOTOR_R_R, PWMFREQ);

  // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // display.clearDisplay();
  // display.setTextSize(1);
  // display.setTextColor(SSD1306_WHITE); 
  // display.setCursor(0,0); 

}

void loop() {
  while(count <= cycles){
    motors.move(PWMFREQ, PWMFREQ);
    delay(2000);
    motors.move(-PWMFREQ, -PWMFREQ);
    delay(2000);
  }
} 
