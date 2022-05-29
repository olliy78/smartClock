/*
    Software for SmartWatch by olliy78

*/
#include "alarm.hpp"



AlarmClock::AlarmClock(){
    speaker = new ATOMSPK();

}

AlarmClock::~AlarmClock(){

}

void AlarmClock::setDataModel(DataModel *m){
    model = m;
    _modelisvalid = true;
}

void AlarmClock::init(){
    if (_modelisvalid){
        _initialized = true;
        debug_println("initialize speaker: ");
        //sleep(100);
        speaker->begin();
        //speaker->playBeep();           
    }
}

void AlarmClock::update(){
    playSound();
}

void AlarmClock::alarmTone(bool b){
    debug_print("Alarm: ");
    debug_println(b);
    _alarmIsOn = b;
    //speaker->playBeep(2000, 500, 1000, false); 
    //speaker->playRAW(chocobo, sizeof(chocobo),true,false);
}

void AlarmClock::playSound(){
    static unsigned long lastMs = 0;
    unsigned long currentMs = millis();
    if (_alarmIsOn == true){
        int delta = currentMs - lastMs;
        if((delta > 0) || (currentMs < lastMs)){          
          if (delta >= 1000){
              lastMs = currentMs;
              return;
          }
          if ((delta > 0 && delta < 90) || (delta > 200 && delta < 290) || (delta > 400 && delta < 490)){
            debug_println("Alarm: c");
            speaker->playBeep(2000, 90, 1000, false);
            return;    
          }
/*          if ((delta > 100 && delta < 190) || (delta > 300 && delta < 390)){
            speaker->playBeep(1000, 90, 1000, false);
            return;    
          }
*/
        }
    }
}