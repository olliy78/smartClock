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
    bool _alarmIsOn = false;
    long _secToNextAlarm = 0;

    void playSound();
    void calcSecToNextAlarm();

public:
    AlarmClock();
    ~AlarmClock();
    void init();
    void update();
    void setDataModel(DataModel *m);
    void alarmTone(bool b);
    bool isAlarmSet();
    bool isAlarmActive();
    bool isSnooze();
    long getAlarmInSec();
    void triggerSnooze();
    void stopAlarm();


};