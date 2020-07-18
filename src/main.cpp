#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>
#include <Arduino.h>
#include <MotorControl.h>
#include <LiftCan.h>
#include<ServoArm.h>
#include<ServoBackFlap.h>
#include<NewPing.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 	-1 // This display does not have a reset pin accessible
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//#define MOTOR_L_R PB_6
//#define MOTOR_L_L PB_7
#define MOTOR_R_L PB_8
#define MOTOR_R_R PB_9

#define PMWFREQ 1000
#define L4 PB1
#define R4 PB0
#define LA PA7
#define RA PA6
#define BACK PA3

#define echo PA5
#define trigger PA4
#define MAX_DISTANCE 300
int count = 0;

LiftCan cl;
ServoArm arm;
ServoBackFlap back;
MotorControl motors;
NewPing sonar(trigger, echo,MAX_DISTANCE );


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
  //motors.begin(MOTOR_L_L, MOTOR_L_R, MOTOR_R_L, MOTOR_R_R, PMWFREQ);
  
  

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.display();
  delay(2000);

  // Displays "Hello world!" on the screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
}

void loop() {
count++;

motors.move(400, 400);

if(sonar.ping_cm() < 30){
  delay(500);
  canDeposit();
  delay(500);
}

if(count== 1000){
  motors.move(0, 0);
  delay(500);
  motors.move(400, -400);
  delay(500);


  count = 0;
}



//delay(5000);
//canDeposit();
//delay(5000);

//arm.close();
//delay(10000);
//arm.bump();
}

