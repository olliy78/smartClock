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
        //model->eepromValid = false;
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
                    //supscribe to toppics
                    mqtt_client->subscribe(MQTT_TIME_TOPPIC);

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
        mqtt_client->loop();
        sendData();
    }
}

void MqttClient::setDataModel(DataModel *m){
    model = m;
    _modelisvalid = true;
}

void MqttClient::callback(char* topic, byte* payload, unsigned int length){
    debug_println("MQTT callback");
    debug_println(topic);
    String topicString = String(topic);
    if (topicString.equals(MQTT_TIME_TOPPIC)){
        String dateTime = String((char*)payload);
        TimeValues mqttTime;
        mqttTime.yyyy = dateTime.substring(0,4).toInt();
        mqttTime.mon = dateTime.substring(5,7).toInt(); 
        mqttTime.dd = dateTime.substring(8,10).toInt();
        mqttTime.hh = dateTime.substring(11,13).toInt(); 
        mqttTime.mm = dateTime.substring(14,16).toInt();
        mqttTime.ss = dateTime.substring(17,19).toInt(); 

        //model->time = mqttTime;
        
        
        debug_println(dateTime.c_str());

        debug_println(dateTime.substring(0,4).c_str()); //year
        debug_println(dateTime.substring(5,7).c_str()); //month
        debug_println(dateTime.substring(8,10).c_str()); //day
        debug_println(dateTime.substring(11,13).c_str()); //hour
        debug_println(dateTime.substring(14,16).c_str()); //minute
        debug_println(dateTime.substring(17,19).c_str()); //second

        
        
    }
}

void MqttClient::sendData(){
    static LightValues oldLight0 = model->eep.elements.lightv[0];
    static LightValues oldLight1 = model->eep.elements.lightv[1];
    unsigned long currentMs = millis();
    static unsigned long lastMs = 0;
    if ((model->mqttOnline && mqtt_client->connected()) && (currentMs - lastMs > 200) || (currentMs < lastMs)){
        lastMs = currentMs;
        
        if (model->eep.elements.lightv[0].white != oldLight0.white || model->eep.elements.lightv[0].color != oldLight0.color || model->eep.elements.lightv[0].intense != oldLight0.intense){
            debug_println("send MQTT data Light 1");
            oldLight0 = model->eep.elements.lightv[0];
            String mqtt_colval = "", mqtt_whval = "";
            mqtt_colval += oldLight0.color;
            mqtt_colval += ", ";
            mqtt_colval += oldLight0.intense;
            mqtt_colval += ", ";
            mqtt_colval += oldLight0.white;
            mqtt_whval += oldLight0.white;
            debug_println(mqtt_whval.c_str());
            if (oldLight0.intense > 0){
                mqtt_client->publish(MQTT_LED1_COLOR, mqtt_colval.c_str());
            } else {
                mqtt_client->publish(MQTT_LED1_WHITE, mqtt_whval.c_str());
            }
        }
        if (model->eep.elements.lightv[1].white != oldLight1.white || model->eep.elements.lightv[1].color != oldLight1.color || model->eep.elements.lightv[1].intense != oldLight1.intense){
            debug_println("send MQTT data Light 1");
            oldLight1 = model->eep.elements.lightv[1];
            String mqtt_colval = "", mqtt_whval = "";
            mqtt_colval += oldLight1.color;
            mqtt_colval += ", ";
            mqtt_colval += oldLight1.intense;
            mqtt_colval += ", ";
            mqtt_colval += oldLight1.white;
            mqtt_whval += oldLight1.white;
            debug_println(mqtt_whval.c_str());
            if (oldLight1.intense > 0){
                mqtt_client->publish(MQTT_LED2_COLOR, mqtt_colval.c_str());
            } else {
                mqtt_client->publish(MQTT_LED2_WHITE, mqtt_whval.c_str());
            }
        } 
    }
}