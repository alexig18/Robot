/**#include <Wire.h>
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

#define TAPE PB12

//number of pings to take the median of for sonar
#define ITERATIONS 5

//60 cm away is the farthest to detect a can 
#define DETECTION_DISTANCE 60 

//number of loops to make through seek before relocating
#define COUNT_IN_SEEK 1000

//number of times to loops through the relocate code before seeking again
#define RELOCATE_LIMIT 1000

//number of times to loop through the relocating turning code before going straight in the relocation 
#define TURN_COUNT 300

#define PMWFREQ 1000
#define L4 PA_0 
#define R4 PA_1
#define LA PA_2
#define RA PA_3
#define BACK PA_8

#define echo PB14
#define trigger PB13
#define MAX_DISTANCE 90

#define COLL_DIST 7

boolean back = false;

LiftCan cl;
ServoArm arm;
//ServoBackFlap back;
MotorControl motors;
NewPing sonar(trigger, echo,MAX_DISTANCE );
void handlein(){

    back = true;
}

void canDeposit(){
   motors.move(0,0);
   arm.close();
   delay(1000);
   cl.lift(20);
   delay(1000);
   arm.bump();
   delay(1000);
   cl.lower(20);
   delay(1000);
   arm.open();
   delay(1000);
}

void setup() {  
  cl.begin(L4, R4);
  arm.begin(LA, RA);

  //back.begin(BACK);
  motors.begin(MOTOR_L_L, MOTOR_L_R, MOTOR_R_L, MOTOR_R_R, PMWFREQ);
  pinMode(TAPE, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(TAPE), handlein, RISING);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 
  // // Displays Adafruit logo by default. call clearDisplay immediately if you don't want this.
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  display.println("initial reading: ");
  display.println(sonar.ping_cm());
  display.display();
  delay(2000);
}


//sonar ping function
int measure()
{
    int dist = sonar.convert_cm(sonar.ping_median(10));
    return dist;
}

//states
const int findCan = 0;
const int navigate = 1;
const int deposit = 2;

int state = findCan;

//substates for findCan. if not in seekState, then it is relocating
bool seekState = true;

//how many times we have looped through the seek state in a row
int seekCount = 0;

String left = "left";
String right = "right";

String relocateDirection = right;
int relocateCount = 0;

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
          motors.move(-400,400);
      } else {
          motors.move(400,400);
      }
  }


int _dist;
int loopCount = 0;



// states
bool homing = false;
bool navMove = false;
int homeCount = 0;

void loop() {
    if (back){
        motors.move(0, 0);
        delay(500);
        motors.move(-450, 450);
        delay(1000);

        back = false;
    }

    motors.move(500,500);

if(sonar.ping_cm() <= 10){
    canDeposit();

}

// switch(state) {
//     case findCan:
//     checkTape();
//     if(seekState) {
//         if(seekCount == 0) {
//             motors.move(400,-400);
//         }
//         int distance = measure();
//         if (distance <= DETECTION_DISTANCE && distance != 0) {
//             motors.move(0,0);
//             seekState = true;
//             seekCount = 0;
//             _dist = distance;
//             state = navigate;
//             navMove = true;
//             display.clearDisplay();
//             display.println("DONE SEEKING, TIME FOR NAVIGATION YEEHAW :)");
//             display.display();
//         } else {
//             seekCount++;
//         }
//         if (seekCount >= COUNT_IN_SEEK) {
//             motors.move(0,0);
//             seekState = false;
//             seekCount = 0;
//             display.clearDisplay();
//             display.println("WE'RE ABOUT TO RELOCATE");
//             display.display();
//         }
//     } else {
//         if(relocateCount == 0) {
//             relocateTurn(relocateDirection);
//             if(relocateDirection.compareTo("left")) {
//                 relocateDirection = right;
//             } else {
//                 relocateDirection = left;
//             }
//         } else if (relocateCount == TURN_COUNT) {
//             motors.move(0,0);
//             motors.move(400,400);
//         } 
//         relocateCount++;
//         if (relocateCount >= RELOCATE_LIMIT) {
//             motors.move(0,0);
//             seekState = true;
//         }
//     }

//     case navigate:
//     checkTape();
//     if(!homing){
//       if(navMove){
//          motors.move(400,400); 
//          navMove = false;
//       }

      
//        if(measure() > _dist){  // enter homing
//          motors.move(0,0);
//          homing = true;
//        }
//     } 
//     else{
//         //seeking
//         if(homeCount == 0) {
//             motors.move(400,-400);
//         }
//         int distance = measure();
//         if (distance <= DETECTION_DISTANCE) {
//             motors.move(0,0);
//             homeCount = 0;
//             navMove = true;
//             homing = false;
//         } else {
//             homeCount++;
//         }

        
//     }
//     if (measure() <= COLL_DIST ){
//       motors.move(0,0);
//       state = deposit;
//       navMove = false;
//       homing = false;
//     }

    
//      _dist = measure();
    
    
//     case deposit:
//     checkTape();
//     canDeposit();
//     state = findCan;
// };

  }










// void dump(){
//   back.open();

//   motors.move(0, 0);
//   delay(1000);
//   motors.move(800, 800);
//   delay(500);
//   motors.move(-800, -800);
//   delay(500);
//   motors.move(0, 0);
//   delay(500);
//   back.close();
**/
