#include <Serv.h>

#define freq 50
PinName _pinS;


Serv::Serv(){}

void Serv::attach(PinName pin){
    
     _pinS = pin;

    pinMode(_pinS, OUTPUT);

}

void Serv::write(int num){
    if (num<0){
        num=0;
    } 
    else if(num>180){
        num = 180;
    }
    int millis = (2500-500)/(180-0)*num+500;
    pwm_start( _pinS,freq,millis,TimerCompareFormat_t::MICROSEC_COMPARE_FORMAT);
 
    
}


