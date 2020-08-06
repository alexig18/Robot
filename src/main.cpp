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
#define Rtrigger PB5

#define echo PB14
#define trigger PB13
#define MAX_DISTANCE 300
int count = 0;
int dMS = 425; // default motor speed

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
int KP = 22; // proportionality constant
int KD = 10; // derivative constant

volatile int SAMPLE = 20; // sample size for eyes LARGER = less sensitive less val oscillations
// SMALLER = more sensitive but larger val oscillations
int bsamples = 170; // sample size for background val - LARGER means steady background val

//states
bool findCan = true;
bool navigate = false;
bool deposit = false;
bool returnHome = false;
bool atHome = false;
bool attack = false;

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
  delay(500);
  cl.lift(10);
  delay(500);
  arm.bump();
  delay(700);
  cl.lower(10);
  delay(500);
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
  int moveinterval = 300; // number of millis in move cycle
  int stopinterval = 225; // number of millis in stop cycle

  int torqueVal = 460; // base move val used to move the motors, if your robot isnt moving in your move interval increase this

  // will rotate until one eye see IR uses inertial controll to slow down its rotation/to pulse rotation
  // DANGER - this offset val HUGELY changes your sensitivity its ideal value is 0 so try to change samplerate, SAMPLE, or bsample first
  int offset = 1;
  // try to minimise this val ^^
  while( Search == true) { 
      // avg[0] >= BACKGROUND-offset && avg[1] >= BACKGROUND-offset &&
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
    int samplerate = 22;
    if (cot == samplerate){
       backgroundMed();
      cot = 0;
    }
    cot++;

      //optional display code to debug
      /**display.setCursor(0, 0);
      display.clearDisplay();
      display.println("Amb: ");
      display.setCursor(40, 0);**/
      //Serial.println(BACKGROUND-offset);
      /**display.setCursor(0, 20);
      display.println("Left           Right");
      display.setCursor(0, 30);**/
      Serial.println(avg[0]);
      //Serial.println(avg[1]);
      //display.setCursor(90,30);
      //display.println(avg[1]);
      //display.display();
    }
    motors.move(0, 0);
}

// navigates robot home
void pidHome() {

  int lastError = 0; // stores last error measured
  std::array<int, 2> avg; // array with left and right IR sensor averages
  int p, g, d, intensity;
  
  // will rotate until one eye see IR uses inertial controll to slow down its rotation/to pulse rotation
  // DANGER - this offset val HUGELY changes your sensitivity its ideal value is 0 so try to change samplerate, SAMPLE, or bsample first
  int offset = 1;

  // step one; find beacon by spinning
  spinSearch();
  motors.move(-dMS, -dMS);
  
  while (true) { // while not running into tape

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
      motors.move(-dMS+g, -dMS-g);
       /**display.setCursor(0, 0);
       display.println("left dty cycle");
       display.setCursor(95, 0);
       display.println(-dMS+g);
       display.setCursor(0, 15);
       display.println("right dty cycle");
       display.setCursor(95, 15);
       display.println(-dMS-g);**/
    } else {
      motors.move(-dMS-g, -dMS+g);
       /**display.setCursor(0, 0);
       display.println("left dty cycle");
       display.setCursor(95, 0);
       display.println(-dMS-g);
       display.setCursor(0, 15);
       display.println("right dty cycle");
       display.setCursor(95, 15);
       display.println(-dMS+g);**/
    }
     Serial.println("pid");
     Serial.println(g);
     //display.display();

    lastError = error;

    // checks if beacon is visible when tape found
    if (digitalRead(TAPE) == 1) {
      state = atHome;
        break;
        
      /**else { // otherwise in wrong place
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
     if (digitalRead(TAPE) == 1){
       motors.move(0,0);
       delay(500);
       motors.move(500,500);
       delay(700);
       motors.move(425,-425);
       delay(2000);
        motors.move(0,0);
     }
   }

  void timeCount(){
    uint32_t timenow = millis();
    if(GLOBALSTART - timenow > 48*1000){
      state = returnHome;
    }
  }

bool leftt = false;
bool rightt = false;


void leftmove(){
  if(digitalRead(Ltrigger) == 0){
  motors.move(-400, 400);
  delay(500);
  motors.move(400, 400);
  delay(500);
  canDeposit();
  }
}

void rightmove(){
  if(digitalRead(Rtrigger) == 0){
  motors.move(400, -400);
  delay(500);
  motors.move(400, 400);
  delay(500);
  canDeposit();
  }
}

void checksonar(){
  int dist = measure();
  Serial.println(dist);
  if(dist <= 13 && dist != 0){
    
    motors.move(0, 0);
    canDeposit();
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
   
 
  Serial1.begin(115200);
  delay(500);
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  /**display.display();
  delay(1000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);**/
}
bool spin = false;
bool relocateTurn = false;
bool relocateStraight = false;

double gtime = millis();

uint32_t relocateTurnTime;
uint32_t relocateStraightTime;
uint32_t timeSeeking;
void loop() {
 

double current;
double start = millis();
double curr;
double target;

if(findCan) {
seekState = true;
current = millis();

if(current - gtime >= 48*1000){
  seekState = false;
  returnHome = true;
}
if (seekState){
motors.move(400, -400);
int dist = measure();
checkTape();
 if (dist <=60 && dist !=0){
   motors.move(-400, -400);
   curr = millis();
   checkTape();
   start = millis();

   while(curr - start <= 1000){
     dist = measure();
     curr = millis();
     checkTape();
     if(dist <=14 && dist !=0){
       motors.move(0, 0);
       canDeposit();
       delay(500);
     }
   }

 }
}
else if(returnHome){
  
start = millis();
int dist = measure();
  while(curr - start <= 2000 && !spin){
    curr = millis();
     dist = measure();
     checkTape();
    motors.move(-420, 420);
    if(dist <=60 && dist != 0 ){
      spin = true;
      break;
    }
  }
  if(spin){
    motors.move(-600, -600);
    dist = measure();
    checkTape();
    if(dist > 60 || dist == 0){
      spin = false;
    }
  }
  dist = measure();
  if(dist <= 16 && dist != 0 ){
    while(true){
    motors.move(0, 0);
    delay(500);
    motors.move(-400, 400);
    delay(5000);
    bflap.open();
    findCan = false;
    }
  }
 

}

 /** if(seekState) {
    motors.move(400,-400);
    distance = measure();
    leftmove();
    rightmove();
    checkTape();
    checksonar();
    if (distance <= DETECTION_DISTANCE && distance != 0) {
      motors.move(0,0);
      attack = true;
      findCan = false;
    } else if ((millis() - timeSeeking) >= SEEK_TIME) {
      motors.move(0,0);
      delay(500);
      motors.move(-400,400);
      leftmove();
  rightmove();
  checkTape();
  checksonar();
      seekState = false;
      relocateTurn = true;
      relocateTurnTime = millis();
    }
  } else if (relocateTurn){
    if((millis() - relocateTurnTime) >= TURN_TIME) {
        motors.move(400,400);
        leftmove();
  rightmove();
  checkTape();
  checksonar();
        relocateTurn = false;
        relocateStraight = true;
        relocateStraightTime = millis();
    } 
  } else if (relocateStraight) {
    if ((millis() - relocateStraightTime >= RELOCATE_TIME)) {
      motors.move(0,0);
      relocateStraight = false;
      seekState = true;
      timeSeeking = millis();
    }
  }
} else if(attack) {
  int reading = measure();
  leftmove();
  rightmove();
  checkTape();
  checksonar();
    if (reading != 0){
      leftmove();
  rightmove();
  checkTape();
  checksonar();
        if (reading >= 60){
          motors.move(750,750);
          leftmove();
  rightmove();
  checkTape();
  checksonar();
        } else if (reading >= 40 && reading < 60){
              motors.move(500,500);
              leftmove();
  rightmove();
  checkTape();
  checksonar();
        }
        else if (reading < 40 && reading >= 24){
          motors.move(450,450);
          leftmove();
  rightmove();
  checkTape();
  checksonar();
        }
        else if(reading<24 && reading >= 15){
          motors.move(400,400);
        }
        else {
          motors.move(0,0);
          canDeposit();
        }
      } else {
        attack = false;
        findCan = true;
      }
  } **/
//}
}
}

