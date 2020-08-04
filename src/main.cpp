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
int dMS = 325; // default motor speed

#define ITERATIONS 5 //number of pings to take the median of for sonar
#define LOOP_TIME  200 // time for each iteration of the main loop, measure using oscilloscope in milliSeconds
//Navigation Constants
#define MAX_DISTANCE 90
#define DETECTION_DISTANCE 50 //the farthest dist to detect a can 
#define COLL_DIST 7
#define SEEK_TIME  2000 // in ms
#define RELOCATE_TIME  3000  //number of times to loops through the relocate code before seeking again (.5)*1000/LOOP_TIME
#define TURN_TIME 4000 // in ms

// IR NAVIGATION STATES -- default when not in IR nav state
bool calibrationMode = true;
bool Search = false;
bool bin = false;

//EDITABLE CONSTANTS
int BACKGROUND = 0; // ambient radiation val
int KP = 27; // proportionality constant
int KD = 10; // derivative constant

volatile int SAMPLE = 220; // sample size for eyes LARGER = less sensitive less val oscillations
// SMALLER = more sensitive but larger val oscillations
int bsamples = 170; // sample size for background val - LARGER means steady background val

//states
const int findCan = 0;
const int navigate = 1;
const int deposit = 2;
const int returnHome = 3;
const int atHome = 4;

int state = findCan;
//substates for findCan. if not in seekState, then it is relocating
bool seekState = true;
//how many times we have looped through the seek state in a row
int seekCount = 0;
String left = "left";
String right = "right";
String relocateDirection = right;
int relocateCount = 0;
int _dist;
int loopCount = 0;
// states
bool homing = false;
bool navMove = false;
int homeCount = 0;
bool test = HIGH;
 int seekTime;
uint32_t loopTime;
bool back = false;

  int distance =0;
uint32_t diff = 0;
uint32_t turnTime =0;
int a2;
int pulseTime = 1000;
bool zero = true;

LiftCan cl;
ServoArm arm;
ServoBackFlap bflap;
MotorControl motors;
NewPing sonar(trigger, echo,MAX_DISTANCE );

uint32_t GLOBALSTART = millis();

void handlein(){
    back = true;
}

//sonar ping function
int measure()
{
    int dist = sonar.convert_cm(sonar.ping_median(ITERATIONS));
    return dist;
}

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

// Sets PID control states for IR navigation
void iRPidState() {
  calibrationMode = true;
  Search = true;
  bin = true;
}

// in-place spin sequence to locate beacon
void spinSearch() {
  //general variables
  std::array<int, 2> avg = avgSamples();
  int cot = 0;
  // inertia control variables
  uint32_t startTime = millis();
  int moveinterval = 400; // number of millis in move cycle
  int stopinterval = 225; // number of millis in stop cycle

  int torqueVal = 430; // base move val used to move the motors, if your robot isnt moving in your move interval increase this

  // will rotate until one eye see IR uses inertial controll to slow down its rotation/to pulse rotation
  // DANGER - this offset val HUGELY changes your sensitivity its ideal value is 0 so try to change samplerate, SAMPLE, or bsample first
  int offset = 4;
  // try to minimise this val ^^
  while(  Search == true) { 
      
    //inertial control code
    uint32_t timeNow = millis();
    if (timeNow-startTime > moveinterval+stopinterval){
      startTime = millis();
    } else if (timeNow-startTime < moveinterval) {
      motors.move(torqueVal,-torqueVal);
    } else if(timeNow-startTime > moveinterval) {
      motors.move(0,0);
    } 

    //readval code
    avg = avgSamples();
    
    // samples background everyonce in a while so while loop isnt occasionally triggered by eye oscillations
    // if this val is increased you increase sensitivity with a fine resolution, opposite is true of decreasing it
    // ensures that uncontrollable light sources won't trigger the eyes
    int samplerate = 25;
    if (cot == samplerate){
       backgroundMed();
      cot = 0;
    }
    cot++;

      //optional display code to debug
      display.setCursor(0, 0);
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
       display.display();
    }
}

// navigates robot home
void pidHome() {

  int lastError = 0; // stores last error measured
  std::array<int, 2> avg; // array with left and right IR sensor averages
  int p, g, d, intensity;
  
  // will rotate until one eye see IR uses inertial controll to slow down its rotation/to pulse rotation
  // DANGER - this offset val HUGELY changes your sensitivity its ideal value is 0 so try to change samplerate, SAMPLE, or bsample first
  int offset = 4;

  // step one; find beacon by spinning
  spinSearch();
  motors.move(-dMS, -dMS);
  
  while (true) { // while not running into tape

    avg = avgSamples();

     display.clearDisplay();
    // NEED TO SCALE TO REASONABLE VALUE FOR MOTOR INPUT
    // left, right
    int error = avg[0]-avg[1];
    int intensity = avg[0]+avg[1];
    p = KP*error; // how fast you react to error 
    d = KD*(error-lastError); 
    g = (p+d)/(sqrt(intensity));
    
    if (avg[0] > BACKGROUND-offset && avg[1] > BACKGROUND-offset) { // makes sure at least one eye is on the beacon at all times, otherwise initiates spin sequence
      spinSearch();
    } else if(avg[0] >= avg[1]){
      motors.move(-dMS+g, -dMS-g);
       display.setCursor(0, 0);
       display.println("left dty cycle");
       display.setCursor(95, 0);
       display.println(-dMS+g);
       display.setCursor(0, 15);
       display.println("right dty cycle");
       display.setCursor(95, 15);
       display.println(-dMS-g);
    } else {
      motors.move(-dMS-g, -dMS+g);
       display.setCursor(0, 0);
       display.println("left dty cycle");
       display.setCursor(95, 0);
       display.println(-dMS-g);
       display.setCursor(0, 15);
       display.println("right dty cycle");
       display.setCursor(95, 15);
       display.println(-dMS+g);
    }
  
     display.println(g);
     display.display();

    lastError = error;

    // checks if beacon is visible when tape found
    if (digitalRead(TAPE) == 1) {
      if (avg[0] < BACKGROUND-offset && avg[1] < BACKGROUND-offset ) { // 
        break;
        state = atHome;
      } /**else { // otherwise in wrong place
        spinSearch();
      }**/
    }

  }

  motors.move(0, 0);
}

// Can dumping sequence
void canDump() {
  // move back a bit
  // open backflap
  // shake a bit? 
}

 void checkTape() {
     if (digitalRead(TAPE)){
       motors.move(0,0);
       motors.move(-400,-400);
       delay(1000);
       motors.move(400,-400);
       delay(1000);
        motors.move(0,0);
     }
   }

  //during findCan state, if you were unsuccessful in seeking a can, move to a new location before seeking again
  void relocateTurn(String relocateDirection) {
      if (relocateDirection.compareTo("left")) {
          motors.move(-dMS,dMS);
      } else {
          motors.move(dMS,-dMS);
      }
  }

  void timeCount(){
    uint32_t timenow = millis();
    if(GLOBALSTART - timenow > 48*1000){
      state = returnHome;
    }
  }

void setup() {  
  cl.begin(L4, R4);
  arm.begin(LA, RA);
  bflap.begin(BACK);
  motors.begin(MOTOR_L_L, MOTOR_L_R, MOTOR_R_L, MOTOR_R_R, PMWFREQ);
  
  pinMode(TAPE, INPUT_PULLUP);
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

void loop() {
  state = returnHome;

 switch(state) {

    case findCan:
    //checkTape();
    timeCount();
    if(seekState) {

     
        if(seekCount == 0) {
            display.clearDisplay();
            display.println("Moving");
            display.display();
             seekTime = millis();
            motors.move(dMS, -dMS);
           // Serial.println("Start Motors");

          
        }
        
         loopTime = millis();
         distance = measure();
        if ( distance <= DETECTION_DISTANCE && distance != 0) {  
            motors.move(0,0);
            seekState = true;
            seekCount = 0;
            _dist = distance;
            state = navigate;
            navMove = true;
            display.clearDisplay();
            display.println("DONE SEEKING, YEEE 🙂");
            display.display();  
            
        } else {
            seekCount++;
        }
        //seekCount++;
        diff = loopTime - seekTime; 
        if (diff >= 3000) {
            motors.move(0,0);
            seekState = false;
            seekCount = 0;
            display.println(diff);
            //  Serial.println("Stop Motors");
            //   Serial.println(diff);
            display.display();

        
        } else if (diff >= pulseTime) {
            if(zero) {
                motors.move(0,0);
                zero = false;
                pulseTime += 200;
            } else {
                motors.move(500,-500);
                zero = true;
                pulseTime += 1000;
            }
        } 
    } else {
        if(relocateCount == 0) {
            //relocateTurn(relocateDirection);
            turnTime = millis();
            if(relocateDirection.compareTo("left")) {
                motors.move(-450,450);
                relocateDirection = right;
            } else {
                motors.move(450,-450);
                relocateDirection = left;
            }
        } 
        loopTime = millis();
         if (loopTime - turnTime >= TURN_TIME) {
            display.clearDisplay();
            motors.move(0,0);
             display.println(loopTime-turnTime);
            display.display();

          
            motors.move(450,450);

                  

        } 
        relocateCount++;
        if (loopTime - turnTime >= RELOCATE_TIME + TURN_TIME)  {
            motors.move(0,0);
            seekState = true;
            relocateCount =0 ;
            display.println(" I'm DONE. _");
            display.display();
            while(1==1){
            }
        } 

  
    }

 

     case navigate:
    // checkTape();
    timeCount();

     if(!homing){
       if(navMove){
          motors.move(dMS,dMS); 
          navMove = false;
       }

        if(measure() > _dist){  // enter homing
          motors.move(0,0);
          homing = true;
        }
     } 
     else{
         //seeking
         if(homeCount == 0) {
             motors.move(dMS,-dMS);
         }
         int distance = measure();
         if (distance <= DETECTION_DISTANCE) {
             motors.move(0,0);
             homeCount = 0;
             navMove = true;
             homing = false;
         } else {
             homeCount++;
         }

     }
     if (measure() <= COLL_DIST ){
       motors.move(0,0);
       state = deposit;
       navMove = false;
       homing = false;
     }

      _dist = measure();

    
     case deposit:
     checkTape();
     canDeposit();
     state = findCan;

  case returnHome:
     // get very stable background value before searching
  for (int i =0;i < 4;i++) {
    backgroundMed();
  }  
  display.setCursor(0, 0);
  display.println("RH");
  display.display();
  // set IR PID states
  iRPidState();
  pidHome();
    //dump cans... or something
   };
}