/*
    Software for SmartWatch by olliy78

*/
#include "mqtt.hpp"

MqttClient::MqttClient(){

}

MqttClient::~MqttClient(){

}

void MqttClient::init(){
    if (_modelisvalid){

        WiFi.begin(model->eep.elements.wifissid, model->eep.elements.wifipsk);

        int conTries = 0;
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            conTries++;
            if (conTries > 20) break;
        }

        if (conTries > 20){
            //Init WiFi as Station, start SmartConfig
            WiFi.mode(WIFI_AP_STA);
            WiFi.beginSmartConfig();

            //Wait for SmartConfig packet from mobile
            Serial.println("Waiting for SmartConfig please use ESP Touch App");
            while (!WiFi.smartConfigDone()) {
            delay(500);
            Serial.print(".");
            }

            Serial.println("");
            Serial.println("SmartConfig received.");

            //Wait for WiFi to connect to AP
            Serial.println("Waiting for WiFi");
            while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
            }
            WiFi.SSID().toCharArray(model->eep.elements.wifissid, 20);
            WiFi.psk().toCharArray(model->eep.elements.wifipsk, 20);
            model->saveEEProm();
        }

        Serial.println("WiFi Connected.");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
        //Serial.println(WiFi.SSID());
        Serial.println(WiFi.psk());
    }
}

void MqttClient::update(){

}

void MqttClient::setDataModel(DataModel *m){
    model = m;
    _modelisvalid = true;
}