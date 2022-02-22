/*
    Software for SmartWatch by olliy78

*/
#pragma once


#include "main.hpp"
#include "model.hpp"

class MqttClient{

    //private
    DataModel *model = nullptr;
    bool _modelisvalid = false;
    
public:
    MqttClient();
    ~MqttClient();
    void init();
    void update();
    void setDataModel(DataModel *m);
};