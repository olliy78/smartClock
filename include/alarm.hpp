/*
    Software for SmartWatch by olliy78

*/
#pragma once


#include "main.hpp"
#include "model.hpp"
#include "speaker.hpp"
#include "sounds.hpp"

class AlarmClock {

    //private
    ATOMSPK *speaker;
    DataModel *model = nullptr;
    bool _modelisvalid = false;
    bool _initialized = false;
    bool _alarmIsOn = false;    //you can hear the sound
    bool _snoozeIsOn = false;   //snooze is active but you can not hear the sound 
    int _soundIsOnSinceSec = 0; //Sound is on since xx sec - it will be deactivated after SOUNDTIME sec
    int _snoozeIsOnSinceSec = 0; //Snooze is on since xx sec - it will be deactivated after SNOOZETIME sec
    long _secToNextAlarm = 0;

    void playSound();
    void calcSecToNextAlarm();

public:
    AlarmClock();
    ~AlarmClock();
    void init();
    void update();
    void setDataModel(DataModel *m);
    bool isAlarmSet();
    bool isAlarmActive();
    bool isSnooze();
    long getAlarmInSec();
    long getSnoozeInSec();
    void triggerSnooze();
    void stopAlarm();


};