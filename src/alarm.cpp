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
    static int oss = 99;
    if (oss != model->time.ss) { // Redraw seconds time every second
        oss = model->time.ss;
        calcSecToNextAlarm();
    }
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
            speaker->playBeep(2000, 200, 1000, true);
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

void AlarmClock::calcSecToNextAlarm(){
    // calculate the seconds of the day today
    long secToday = 0;
    long secUntilAlarm[2] = {0, 0};
    
    secToday += model->time.hh * 3600;
    secToday += model->time.mm * 60;
    secToday += model->time.ss;
    //for each of the alarm calculate the Seconds of the day
    for (int i=0; i<2; i++){
        long secAlarmDay = 0;
        secAlarmDay += model->eep.elements.alarmv[i].hh *3600;
        secAlarmDay += model->eep.elements.alarmv[i].mm *60;
        secUntilAlarm[i] = secAlarmDay - secToday;
        //locking for the next maching day of week      
        int scanDay;
        if (secUntilAlarm[i] < 0){ //today its to late
            scanDay = model->time.dow + 1;
            if (scanDay == 7) scanDay = 0;
            secUntilAlarm[i] += 86400;    //add 24h
        } else {                    //alarm will happen today
            scanDay = model->time.dow;
        }
        for (int d=0; d<7; d++){
            if (model->eep.elements.alarmv[i].day[scanDay]){
                break;              //there is an alarm on this day - search is done
            }
            scanDay++;      //go to the next day
            if (scanDay == 7) scanDay = 0;
            secUntilAlarm[i] += 86400;    //add 24h

        } 
    }
    if (secUntilAlarm[0] < secUntilAlarm[1]){
        _secToNextAlarm = secUntilAlarm[0];
    } else {
        _secToNextAlarm = secUntilAlarm[1];
    }
}

//returns false if no Alarm is set
bool AlarmClock::isAlarmSet(){
    bool retval = false;
    for (int m=0; m<2; m++){
        for (int n=0; n<7; n++){
            if (model->eep.elements.alarmv[m].day[n]){
                retval = true;
            }
        }
    }
    return retval;
}

//returns true while Alarm sounds
bool AlarmClock::isAlarmActive(){
    return true;
}

//returns true while Alarm is in snooze mode
bool AlarmClock::isSnooze(){
    return false;
}

long AlarmClock::getAlarmInSec(){
    return _secToNextAlarm;    
}

void AlarmClock::triggerSnooze(){
    debug_println("Snooze pressed");

}

void AlarmClock::stopAlarm(){
    debug_println("Alarm stop pressed");
}
