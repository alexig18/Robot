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

// STATES
bool calibrationMode = true;
bool returnHome = false;
bool atHome = false;

//EDITABLE CONSTANTS
int BACKGROUND = 0;
int KP = 1; // proportionality constant
int KD = 1; // derivative constant
volatile int SAMPLE = 200;

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

  int bsamples = 100;
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
  returnHome = false;
  atHome = true;
}

// Can dumping sequence
void canDump() {
  // move back a bit
  // open backflap
  // shake a bit? 
}
boolean Sent = true;
int Minl = 2000;
  int Minr = 2000;


void loop() {

  
  std::array<int, 2> avg = avgSamples();

  
  /**motors.move(-300, 300);
  for(int i =0; i<200; i++){
    avg = avgSamples();
    if(avg[0] < Minl) Minl = avg[0];
    if(avg[1] < Minr) Minr = avg[1];
  }

  motors.move(300, -300);
}

  avg = avgSamples();

  while(avg[0] >= Minl && avg[1] >= Minr && Sent == true){ // will rotate until both eyes see IR
     avg = avgSamples();

      display.setCursor(0, 0);
      display.clearDisplay();

     /** if(cot == 15){
        backgroundMed();
        
        //display.println("b-set");
        //display.display();
        cot = 0;
      }
      //display.println(BACKGROUND-4);
      display.println(Minl);
      display.setCursor(40, 0);
      display.println(Minr);
      display.setCursor(0, 20);
      display.println(avg[0]);

      display.println(avg[1]);

      display.display();
      //cot++;

    }
   
    motors.move(0, 0);
     Sent = false;
    display.clearDisplay();
        display.println("FOUND");
        display.display();**/

  //convolution code
  /**int SAMPLE = 70;
    int Laverage, Raverage, start_time, elapsed_time =0;

    int * RIRdata = new int[SAMPLE];
    int *  LIRdata = new int[SAMPLE];
    int * LWaveform = new int[SAMPLE];
    int * RWaveform = new int[SAMPLE];
    int * Ronekcorr = new int[SAMPLE];
    int * Lonekcorr = new int[SAMPLE];
    int * onek = new int[SAMPLE*2];
    
    start_time=millis();
    for (int i=0;i<SAMPLE; i++)
      {
      LIRdata[i] = analogRead(Leye);
      RIRdata[i] = analogRead(Reye);
      }
    elapsed_time=millis()-start_time; //count the number of milliseconds to acquire #SAMPLEs
    for (int i=0;i<SAMPLE; i++)
      {
      Laverage+=LIRdata[i];
      Raverage+=RIRdata[i];
      }
    Laverage = (long) (double)Laverage/SAMPLE;
    Raverage = (long) (double)Raverage/SAMPLE; //DC average of reading
    double onekT= SAMPLE/(double) elapsed_time;
    for (int i=0;i<2*SAMPLE;i++)
      {
      onek[i]=sin(i*TWO_PI/onekT);
      //generate 1khz waveforms
      }
    for (int i=0;i<SAMPLE; i++)
      {
      LWaveform[i]=LIRdata[i]-(int)Laverage;  
      RWaveform[i]=RIRdata[i]-(int)Raverage; //subtract DC, square, and sum
      }
    for (int k=0; k<SAMPLE; k++)
    {
      Lonekcorr[k]=0;
      Ronekcorr[k]=0;
      for (int i=0;i<SAMPLE;i++)
      {
      Lonekcorr[k]+=LWaveform[i]*onek[k+i];
      Ronekcorr[k]+=RWaveform[i]*onek[k+i];  //cross correlate waveform with 1 and 10k
      }
    }
    int Lmax_one=0;
    int Rmax_one=0;
    for (int i=0;i<SAMPLE;i++)
      {
      if (Lonekcorr[i]>Lmax_one) Lmax_one=Lonekcorr[i];
      if (Ronekcorr[i]>Rmax_one) Rmax_one=Ronekcorr[i];// find correlation peaks
      }
  //  threshold = analogRead(DETECT_THRESHOLD);
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(Rmax_one);
    display.setCursor(40,0);
    display.println(Lmax_one);
   // display.println("Thresh= ");
    
    //display.println((int) threshold/50);

   // if ((max_one*500<FREQ_THRESH_ONE*threshold)&&(max_ten*500<FREQ_THRESH_TEN*threshold))
    //{
    //display.println("No signal");
   /// }
    
    

       // display.println("1 kHz");
      
    

    display.display();**/



  //average detection code vv
  display.setCursor(0, 0);
  display.clearDisplay();

  backgroundMed();
  returnHome = true;
  

  while(returnHome){
    motors.move(0, 0);
    display.setCursor(0, 0);
    display.clearDisplay();

    if(Sent == true){
      motors.move(-325,325);
      display.println("Searching");
      display.display();
    }

    std::array<int, 2> avg = avgSamples();
    int cot = 0;

    // inertia control variables
    double startTime = millis();
    int interval = 400; // number of millis in a cycle

    while(avg[0] >= BACKGROUND-3 && avg[1] >= BACKGROUND-3 && Sent == true){ // will rotate until both eyes see IR
      // try both: stop when vals are almost equal, stop when one detector falls bellow background
      double timeNow = millis();
      if (timeNow-startTime > interval/3) {
        motors.move(-325,325);
      } else if(timeNow-startTime < interval/3) {
        motors.move(0,0);
      } else if (timeNow-startTime > interval) {
        startTime = millis();
      }
      avg = avgSamples();

      display.setCursor(0, 0);
      display.clearDisplay();

      if(cot == 23){
        backgroundMed();
        
        //display.println("b-set");
        //display.display();
        cot = 0;
      }
      display.println(BACKGROUND);

      display.println(avg[0]);
      display.setCursor(20, 20);
      display.println(avg[1]);

      display.display();
      cot++;
      count++;
    }
    
    motors.move(0, 0);
    
      display.clearDisplay();
        display.println("FOUND");
        display.display();

    Sent = false;

    //pidHome();
  }

  if(atHome) {
    // create can dumping sequence
    // open backflap, shake a bit (?) etc.
  }
}