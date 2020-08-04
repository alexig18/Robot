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
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
  
  pinMode(TAPE, INPUT_PULLUP);
  pinMode(Ltrigger, INPUT_PULLUP);
  pinMode(Rtrigger, INPUT_PULLUP);
  pinMode(Leye, INPUT);
  pinMode(Reye, INPUT);

 // display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
 /** display.display();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);**/



}

// STATES
bool calibrationMode = true;
bool returnHome = false;
bool atHome = false;
boolean Search = true;
bool bin = true;

//EDITABLE CONSTANTS
int BACKGROUND = 0; // ambient radiation val
int KP = 27; // proportionality constant
int KD = 10; // derivative constant

volatile int SAMPLE = 220; // sample size for eyes LARGER = less sensitive less val oscillations
// SMALLER = more sensitive but larger val oscillations
int bsamples = 170; // sample size for background val - LARGER means steady background val

// IR control
// collects IR samples from IR Sensor and get average
// returns an array with average of left eye in position 0, right eye in position 1
std::array<int, 2> avgSamples() {
int ltot = 0;
int rtot = 0;
  std::array<int, 2> avg;

  for(int i=0; i<SAMPLE; i++){
    ltot = ltot + analogRead(Leye);
    rtot = rtot + analogRead(Reye);
  }
   avg[0] = ltot/SAMPLE;
   avg[1] = rtot/SAMPLE;

  return avg;
}

//Updates BACKGROUND constant with an average of bsample size
void backgroundMed(){
  BACKGROUND = 0;
  for(int i =0; i<bsamples; i++){
    BACKGROUND = (BACKGROUND + (analogRead(Leye)+analogRead(Reye))/2);
  }
  BACKGROUND = BACKGROUND/bsamples;
}

// PID control for IR navigation
void pidHome() {
  int lastError = 0; // stores last error measured
  std::array<int, 2> avg; // array with left and right IR sensor averages
  int p, g, d, intensity;
  motors.move(-400, -400);
  while(digitalRead(TAPE) == 0) { // while not running into tape
    
    avg = avgSamples();
    //display.clearDisplay();
    // NEED TO SCALE TO REASONABLE VALUE FOR MOTOR INPUT
    // left, right
    int error = avg[0]-avg[1];
    int intensity = avg[0]+avg[1];
    p = KP*error; // how fast you react to error 
    d = KD*(error-lastError); 
    g = (p+d)/(sqrt(intensity));
    if(avg[0] >= avg[1]){
        motors.move(-350+g, -350-g);

       /** display.setCursor(0, 0);
    display.println("left dty cycle");
    display.setCursor(95, 0);
    display.println(-350+g);
   display.setCursor(0, 15);
  display.println("right dty cycle");
    display.setCursor(95, 15);
    display.println(-350-g);**/
    } 
    
    else{
        motors.move(-350-g, -350+g);

       /** display.setCursor(0, 0);
    display.println("left dty cycle");
    display.setCursor(95, 0);
    display.println(-350-g);
   display.setCursor(0, 15);
  display.println("right dty cycle");
    display.setCursor(95, 15);
    display.println(-350+g);**/
    }
  
    
  
   // display.println(g);
   // display.display();

    lastError = error;
    g=0;
    p=0;
    d=0;
  }

  returnHome = false;
  atHome = true;

  motors.move(0, 0);
}

// Can dumping sequence
void canDump() {
  // move back a bit
  // open backflap
  // shake a bit? 
}

void loop() {

 
  canDeposit();
  delay(3000);

  //state searches and returns to bin with can payload
  while(returnHome){
     //get very stable background value before searching
  for(int i =0;i<4;i++){
    backgroundMed();
  }
    motors.move(0, 0);
    //display.setCursor(0, 0);

    //general variables
    std::array<int, 2> avg = avgSamples();
    int cot = 0;

    // inertia control variables
    double startTime = millis();
    int moveinterval = 400; // number of millis in move cycle
    int stopinterval = 225; // number of millis in stop cycle

    int torqueVal = 430; // base move val used to move the motors, if your robot isnt moving in your move interval increase this

    // will rotate until one eye see IR uses inertial controll to slow down its rotation/to pulse rotation
    // DANGER - this offset val HUGELY changes your sensitivity its ideal value is 0 so try to change samplerate, SAMPLE, or bsample first
    int offset = 4;
    // try to minimise this val ^^
    while(( avg[0] >= BACKGROUND-offset && avg[1] >= BACKGROUND - offset) && Search == true){ 
      //inertial controll code
      double timeNow = millis();
      if(timeNow-startTime > moveinterval+stopinterval){
        startTime = millis();
      }
      else if (timeNow-startTime < moveinterval) {
        motors.move(torqueVal,-torqueVal);
      } else if(timeNow-startTime > moveinterval) {
        motors.move(0,0);
      } 

      //readval code
      avg = avgSamples();
      //samples background everyonce in a while so while loop isnt occasionally triggered by eye oscillations
      //if this val is increased you increase sensitivity with a fine resolution, opposite is true of decreasing it
      int samplerate = 25;
      if(cot == samplerate){
        backgroundMed();
        cot = 0;
      }
       cot++;

      //optional display code to debug
      /**display.setCursor(0, 0);
      display.clearDisplay();
      display.println("Amb: ");
      display.setCursor(40, 0);
      display.println(BACKGROUND-offset);
      display.setCursor(0, 20);
      display.println("Left           Right");
      display.setCursor(0, 30);
      display.println(avg[0]);
      display.setCursor(90,30);
      display.println(avg[1]);
      display.display();**/
     
    }
    motors.move(0, 0);
    
    //optional found display to visually see when the robot has seen the beacon
    /** display.clearDisplay();
      display.println("FOUND");
        display.println(avg[0]);
        display.println(avg[1]);
        display.display();**/
   
   //ensures you never enter search loop after you have seen the beacon
   // we should test/ try entering this if we loose sight of the beacon later
   Search = false;


    
    //starts the move towards the bin
    if(bin == true){
    pidHome();
    bin = false;
    }
  }
  
  if(atHome) {
    // create can dumping sequence
    // open backflap, shake a bit (?) etc.
  }
}