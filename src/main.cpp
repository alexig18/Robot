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
#define Leye PA5
#define Reye PA4

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

int LEFTSPEED; // keep track of speed of motor rotation 'speeds'
int RIGHTSPEED;


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
  pinMode(Leye, INPUT);
  pinMode(Reye, INPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.display();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
}

bool calibrationMode = true;
bool returnHome = false;
int BACKGROUND = 0;
// the reading from the farthest dist from the beacon
int MAXLIGHT = 730;

int SAMPLES = 70;
//int samplewave[];
int tot = 0;

// IR control
// collects IR samples from IR Sensor and get average
// returns an array with average of left eye in position 0, right eye in position 1
int * avgSamples() {
  int leftTot;
  int rightTot;
  for(int i=0; i<SAMPLES; i++){
    leftTot = leftTot + analogRead(Leye);
    rightTot = rightTot + analogRead(Reye);
  }
  int avg[2] = {leftTot/SAMPLES, rightTot/SAMPLES};
  return avg;
}

int KP = 1; // proportionality constant
int KD = 1; // derivative constant

// PID control for IR navigation
void pidHome() {
  int lastError = 0; // stores last error measured
  int* avg; // array with left and right IR sensor averages
  int p, g, d, intensity;
  
  while(!digitalRead(TAPE)) { // while not running into tape
    avg = avgSamples();
    // NEED TO SCALE TO REASONABLE VALUE FOR MOTOR INPUT
    // left, right
    int error = avg[0]-avg[1];
    int intensity = avg[0]+avg[1];
    p = KP*error; // how fast you react to error 
    d = KD*(error-lastError); 
    g = (p+d)/(1+sqrt(intensity));
    motors.move(motors.leftSpeed()+g, motors.rightSpeed()-g);
    
    lastError = error;
    }
}

void loop() {
  display.setCursor(0, 0);
  display.clearDisplay();

  for(int i=0; i<SAMPLES; i++){
    tot = tot + analogRead(Leye);
  }
  int average = tot/SAMPLES;

  display.println("Average: ");
  display.println(average);
  display.display();
  delay(50);

  average = 0;
  tot = 0;
  count = 0;
  while(calibrationMode && count < 100) {
    if(BACKGROUND == 0) {
      BACKGROUND = (analogRead(Leye)+analogRead(Reye))/2;
    } else {
      BACKGROUND = (BACKGROUND + analogRead(Leye)+analogRead(Reye))/3;
    }
    count++;
  }
  display.println("amb: ");
  display.print(BACKGROUND);
  display.display();

  calibrationMode = false;
  returnHome = true;

  while(returnHome){
    motors.move(0, 0);
    // true = left || false = right
    // boolean triggered;

    motors.move(-450, 450);
    while(analogRead(Leye) >= BACKGROUND  &&  analogRead(Reye) >= BACKGROUND ){ // will rotate until both eyes see IR
      // try both: stop when vals are almost equal, stop when one detector falls bellow background
    }
    motors.move(0, 0);

    pidHome();

    //TEST DTYSUM ADD TO ONE WHEEL SUB FROM OTHER

  }
}