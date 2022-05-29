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

    void playSound();

public:
    AlarmClock();
    ~AlarmClock();
    void init();
    void update();
    void setDataModel(DataModel *m);
    void alarmTone(bool b);


};