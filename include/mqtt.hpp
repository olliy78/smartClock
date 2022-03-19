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
    //static DataModel *model;
    bool _modelisvalid = false;
    bool _initialized = false;
    char msg[MSG_BUFFER_SIZE];
    //WiFiClient espClient;
    PubSubClientTools *mqtt;
    //JsonDoc //size calcolated with https://arduinojson.org/v6/assistant/
    StaticJsonDocument<400> jsonDoc;

    LightValues hsbStrToLightval(const char* hsbStr);

    

    
public:
    MqttClient();
    ~MqttClient();
    void init();
    void update();
    void setDataModel(DataModel *m);
    void setMQTTClient(PubSubClient *c);
    void sendData();
    void mqttSubsribe();
    void callback(char* topic, byte* payload, unsigned int length);
};