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
        //debug_println("initialize speaker: ");
        //sleep(100);
        speaker->begin();
        //speaker->playBeep();           
    }
}

void AlarmClock::update(){
    playSound();                  //plays the sound if necessary
    static int oss = 99;
    if (oss != model->time.ss) { // do every second:
        oss = model->time.ss;
        calcSecToNextAlarm();   // calculate time until the next alarm and check if a alarm event has happen
        if (_alarmIsOn) _soundIsOnSinceSec++;                       //count the seconds the alarm is on
        if (_soundIsOnSinceSec > SOUNDTIME) _alarmIsOn = false;     //deactivate the alarm sound after SOUNDTIME sec
        if (_alarmIsOn){
            debug_println("BEEP BEEP");
        }
        //snooze stuff
        if (_snoozeIsOn) _snoozeIsOnSinceSec++;                     //count the sec snooze is on
        if (_snoozeIsOnSinceSec > SNOOZETIME){
            _alarmIsOn = true;    //if snooze time is done switch on the alarm
            _snoozeIsOn = false;    // and switch snooze mode off
        } 
    }
}


//plays beep sound
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
            if (_soundIsOnSinceSec < 60){
                if ((delta > 0 && delta < 90)){
                    speaker->playBeep(2000, 200, VOLUMEN / 8, true);
                    return;    
                }
            } else if (_soundIsOnSinceSec < 120){
                if ((delta > 0 && delta < 90) || (delta > 200 && delta < 290)){
                    speaker->playBeep(2000, 200, VOLUMEN / 4, true);
                    return;    
                }
            } else {
                if ((delta > 0 && delta < 90) || (delta > 200 && delta < 290) || (delta > 400 && delta < 490)){
                    speaker->playBeep(2000, 200, VOLUMEN, true);
                    return;    
                }
            }

        }
    }
}

//calculate the seconds until the next alarm and check if an alarm meanwhile happen
void AlarmClock::calcSecToNextAlarm(){
    // calculate the seconds of the day today
    long secToday = 0;
    long secUntilAlarm[2] = {0, 0};    
    secToday += model->time.hh * 3600;
    secToday += model->time.mm * 60;
    secToday += model->time.ss;
    // for each alarm calculate seconds until the next event, if 0 is crossed activate the alarm
    for (int i=0; i<2; i++){
        //for each of the alarm calculate the Seconds of the day
        long secAlarmDay = 0;
        secAlarmDay += model->eep.elements.alarmv[i].hh *3600;
        secAlarmDay += model->eep.elements.alarmv[i].mm *60;
        secUntilAlarm[i] = secAlarmDay - secToday;          //that might be negative
        //check if an alarm event happend in the last 3sec - deactivation of the alarm typically tooks longer than 3sec
        if (secUntilAlarm[i] <= 0 && secUntilAlarm[i] >= -2){ 
            //check if the alarm for today is active
            if (model->eep.elements.alarmv[i].day[model->time.dow]){
                debug_println("Alarm Event!!");
                _alarmIsOn = true;  //activate the alarm sound
                _soundIsOnSinceSec = 0; //set the time counter to 0 - the alarm will deactivated after SOUNDTIME sec
            }
        }
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
                //debug_print(i);
                //debug_print("# today is dow ");
                //debug_print(model->time.dow);
                //debug_print(" next alarm is ");
                //debug_println(scanDay);
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
    return _alarmIsOn;
}

//returns true while Alarm is in snooze mode
bool AlarmClock::isSnooze(){
    return _snoozeIsOn;
}

long AlarmClock::getAlarmInSec(){
    return _secToNextAlarm;    
}

long AlarmClock::getSnoozeInSec(){
    return (long) SNOOZETIME - _snoozeIsOnSinceSec;
}

void AlarmClock::triggerSnooze(){
    debug_println("Snooze pressed");
    _alarmIsOn = false;
    _snoozeIsOn = true;
    _snoozeIsOnSinceSec = 0;

}

void AlarmClock::stopAlarm(){
    debug_println("Alarm stop pressed");
    _alarmIsOn = false;
    _snoozeIsOn = false;
}
