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

#define MOTOR_L_R PA_7 // left motor right pin
#define MOTOR_L_L PA_6 // left motor left pin
#define MOTOR_R_R PB_1 // right motor forward
#define MOTOR_R_L PB_0 // right motor backward
#define PWMFREQ 1000
#define cycles 10


MotorControl motors = MotorControl();
int count;

void setup() {  
  timerSetup();
  motors.begin(MOTOR_L_L, MOTOR_L_R, MOTOR_R_L, MOTOR_R_R, PWMFREQ);

   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(SSD1306_WHITE); 
   display.setCursor(0,0); 
  display.println("Working");
  display.display();

  delay(2000);
}

void loop() {
  display.clearDisplay();
  display.println("wow");
  display.display();
display.setCursor(0,0); 

  
    motors.move(250, 250);
    delay(5000);
    motors.move(-250, -250);
    delay(5000);
    motors.goToAngle("L", 5000);
    delay(5000);

  display.clearDisplay();
  display.println("damn");
  display.display();
} 

// void timerSetup() {
//     for (int i = 1; i <= 4; i++) {
//         HardwareTimer timer(i);
//         timer.pause();
//         timer.setPeriod(20000);
//         timer.refresh();
//         timer.resume();
//     }
// }