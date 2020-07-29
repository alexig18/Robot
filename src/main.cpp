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

#define MOTOR_L_R PB_1
#define MOTOR_L_L PB_0
#define MOTOR_R_L PA_7
#define MOTOR_R_R PA_6

#define PMWFREQ 1000
#define L4 PA_0
#define R4 PA_1
#define LA PA_2
#define RA PA_3
#define BACK PA_8

#define TAPE PB12

#define Ltrigger PB4
#define Rtrigger PB3

#define echo PB14
#define trigger PB13
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
  motors.begin(MOTOR_L_L, MOTOR_L_R, MOTOR_R_L, MOTOR_R_R, PMWFREQ);
  
  pinMode(Ltrigger, INPUT_PULLUP);
  pinMode(Rtrigger, INPUT_PULLUP);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
}

void loop() {
display.setCursor(0,0);

  if(digitalRead(Ltrigger)){
    display.println("left can!");
    display.display();
    delay(1000);
    display.clearDisplay();

  }
  display.setCursor(0,0);

   if(digitalRead(Rtrigger)){
    display.println("right can!");
    display.display();
    delay(1000);
    display.clearDisplay();
  }
}



//delay(5000);
//canDeposit();
//delay(5000);

//arm.close();
//delay(10000);
//arm.bump();
//}

