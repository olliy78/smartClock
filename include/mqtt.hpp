/*
    Software for SmartWatch by olliy78

*/
#pragma once


#include "main.hpp"
#include "model.hpp"

#define MSG_BUFFER_SIZE 50

class MqttClient{

    //private
    PubSubClient *mqtt_client;
    DataModel *model = nullptr;
    bool _modelisvalid = false;
    bool _initialized = false;
    char msg[MSG_BUFFER_SIZE];
    WiFiClient espClient;
    PubSubClientTools *mqtt;

    static void callback(char* topic, byte* payload, unsigned int length);

    
public:
    MqttClient();
    ~MqttClient();
    void init();
    void update();
    void setDataModel(DataModel *m);
    void sendData();
};