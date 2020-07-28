#ifndef CanNavigation_h
    #define CanNavigation_h
    #include "Arduino.h"
    #include "ServoArm.h"
    #include "NewPing.h" 
    #include "MotorControl.h"
    class CanNavigation
    {
        public:
        CanNavigation(int echo,int trig,int max,MotorControl motors);
        bool findCan();
        private:
        // the underscore helps show the variable is private
        NewPing* _sonar;
        MotorControl _motors;
        int _distance;
        int Measure(); 
        // to locate something within sonar field of robot
        bool seek();
        // to confirm whether something is a can or not
        void move(String direction,int tim);
        // navigates to can
        bool pingDelay (int interval, int threshold);
    };
    #endif