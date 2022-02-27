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
        
        mqtt_client = new PubSubClient(espClient);
        mqtt = new PubSubClientTools(*mqtt_client);
        
        _initialized = true;
    }
}

void MqttClient::update(){
    static bool wifiInfoDisplayed = false;
    if (_initialized && WiFi.status() != WL_CONNECTED) {
        unsigned long currentMs = millis();
        static unsigned long lastMs = 0;
        static int conTries = 0;
        if((currentMs - lastMs > 500) || (currentMs < lastMs)){
            //Serial.println("ping");
            lastMs = currentMs;
            if (WiFi.status() != WL_CONNECTED) {
                Serial.print(".");
                conTries++;
            }
            if (conTries == 20){
                //Init WiFi as Station, start SmartConfig
                WiFi.mode(WIFI_AP_STA);
                WiFi.beginSmartConfig();

                //Wait for SmartConfig packet from mobile
                Serial.println("Waiting for SmartConfig please use ESP Touch App");
            }
            if (conTries > 20){
                if (WiFi.smartConfigDone()){
                    conTries = 0;
                    WiFi.SSID().toCharArray(model->eep.elements.wifissid, 20);
                    WiFi.psk().toCharArray(model->eep.elements.wifipsk, 20);
                    model->eepromValid = false; //write to EEProm soon
                }
            }
        }
    } else if (wifiInfoDisplayed == false){         //display only once after connection
        wifiInfoDisplayed = true;
        model->wifiOnline = true;
        Serial.print("WiFi Connected SSID:");
        Serial.println(WiFi.SSID());
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());
    }
    // connect to MQTT server
    if (model->wifiOnline == true && model->mqttOnline == false){
        //WiFiClient espClient;
        //client = new PubSubClient(espClient);
        
        mqtt_client->setServer(MQTT_SERVER, 1883);  //Sets the server details
        mqtt_client->setCallback(&MqttClient::callback); //Sets the message callback function
        if (!mqtt_client->connected()){
            unsigned long currentMs = millis();
            static unsigned long lastMs = 0;
            if((currentMs - lastMs > 5000) || (currentMs < lastMs)){
                lastMs = currentMs;
                //MQTT Client-ID
                if (mqtt_client->connect(MQTT_CLIENT_ID)) {
                    debug_println("MQTT connected");
                    model->mqttOnline = true;
                    //String stateStr = "stat/"+ MQTT_CLIENT_ID + "/STAT";
                    mqtt_client->publish(MQTT_STAT, "ONLINE");  //Publishes a message to the specified topic

                } else {
                    debug_print("failed, rc=");
                    debug_print(mqtt_client->state());
                    debug_println(" try again in 5 seconds");
                    model->mqttOnline = false;
                }
            }
        }
    }
    if (model->wifiOnline == true && model->mqttOnline == true){
        if (!mqtt_client->connected()){
            debug_println("MQTT Broker error - trying to reconnect...");
            model->mqttOnline = false;
        }
        sendData();
    }
}

void MqttClient::setDataModel(DataModel *m){
    model = m;
    _modelisvalid = true;
}

void MqttClient::callback(char* topic, byte* payload, unsigned int length){
    debug_println("MQTT callback");
}

void MqttClient::sendData(){
    static DataElements oldData = model->eep.elements;
    unsigned long currentMs = millis();
    static unsigned long lastMs = 0;
    if ((model->mqttOnline && mqtt_client->connected()) && (currentMs - lastMs > 200) || (currentMs < lastMs)){
        lastMs = currentMs;
        if (model->eep.elements.lightv[0].white != oldData.lightv[0].white || model->eep.elements.lightv[0].color != oldData.lightv[0].color || model->eep.elements.lightv[0].intense != oldData.lightv[0].intense){
            debug_println("send MQTT data");
            String mqtt_val = "";
            mqtt_val += (int)model->eep.elements.lightv[0].color;
            mqtt_val += ", ";
            mqtt_val += (int)model->eep.elements.lightv[0].intense;
            mqtt_val += ", ";
            mqtt_val += (int)model->eep.elements.lightv[0].white;
            debug_println(mqtt_val.c_str());
            mqtt_client->publish("cmnd/SchrankJonas/HSBColor", mqtt_val.c_str());
            oldData = model->eep.elements;
        } 
    }
}