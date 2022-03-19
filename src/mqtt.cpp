/*
    Software for SmartWatch by olliy78

*/
#include "mqtt.hpp"


MqttClient::MqttClient(){

}

MqttClient::~MqttClient(){

}

void MqttClient::setDataModel(DataModel *m){
    model = m;
    _modelisvalid = true;
}

void MqttClient::setMQTTClient(PubSubClient *c){
    mqtt_client = c;
    mqtt = new PubSubClientTools(*mqtt_client);

}

void MqttClient::init(){
    if (_modelisvalid){

        WiFi.begin(model->eep.elements.wifissid, model->eep.elements.wifipsk);
        
        //mqtt_client = new PubSubClient(espClient);
        //mqtt = new PubSubClientTools(*mqtt_client);
        
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
        //mqtt_client->setCallback(&MqttClient::callback); //Sets the message callback function
        if (!mqtt_client->connected()){
            unsigned long currentMs = millis();
            static unsigned long lastMs = 0;
            if((currentMs - lastMs > 5000) || (currentMs < lastMs)){
                lastMs = currentMs;
                //MQTT Client-ID
                if (mqtt_client->connect(MQTT_CLIENT_ID)) {
                    debug_println("MQTT connected");
                    model->mqttOnline = true;       //set the connected state to the model
                    mqtt_client->publish(MQTT_STAT, "ONLINE");  //Publishes a message to the specified topic
                    mqttSubsribe(); //supscribe to toppics
                } else {
                    debug_print("failed, rc=");
                    debug_print(mqtt_client->state());
                    debug_println(" try again in 5 seconds");
                    model->mqttOnline = false;      // set the not connected state to the model
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

void MqttClient::mqttSubsribe(){
    mqtt_client->subscribe(MQTT_TIME_TOPPIC);
    mqtt_client->subscribe(MQTT_LED1_STATE);
    mqtt_client->subscribe(MQTT_LED2_STATE);

}


void MqttClient::callback(char* topic, byte* payload, unsigned int length){
    debug_print("MQTT callback toppic: ");
    debug_println(topic);
    char payloadBuf[256] = "";
    if (length < 256){
        strncpy(payloadBuf, (char*)payload, length);
    }
    debug_println(payloadBuf);
    String topicString = String(topic);
    String payloadString = String(payloadBuf);
    if (topicString.equals(MQTT_TIME_TOPPIC)){
        //the date String is formated as ISO8601 "2022-03-19T08:49:00.005031" it will be cuttet char wise
        struct tm timeinfo;
        timeinfo.tm_year = payloadString.substring(0,4).toInt() - 1900;
        timeinfo.tm_mon = payloadString.substring(5,7).toInt() -1;
        timeinfo.tm_mday = payloadString.substring(8,10).toInt();
        timeinfo.tm_hour = payloadString.substring(11,13).toInt();
        timeinfo.tm_min = payloadString.substring(14,16).toInt();
        timeinfo.tm_sec = payloadString.substring(17,19).toInt();
        time_t time = mktime(&timeinfo);        //create a time_t object (seconds since xxx)
        model->setRTC(time);                    //set to the model
    }
    unsigned long currentMs = millis();
    if ((topicString.equals(MQTT_LED1_STATE) || topicString.equals(MQTT_LED2_STATE)) && (currentMs - model->lastSendTime > 1000)){
        //{"POWER":"ON","Dimmer":33,"Color":"0000005400","HSBColor":"231,64,0","White":33,"CT":153,"Channel":[0,0,0,33,0]}
        // Deserialize the JSON document
        DeserializationError error = deserializeJson(jsonDoc, payloadBuf);
        // Test if parsing succeeds.
        if (error) {
            debug_print("deserializeJson() failed: ");
            debug_println(error.f_str());
            return;
        }
        const char* colorStr = jsonDoc["HSBColor"]; // contains a List of 3 comma separated values "HSBColor":"231,64,0"
        LightValues newval = hsbStrToLightval(colorStr);
        String poweStr = String((const char*)jsonDoc["POWER"]);
        bool power = poweStr.equalsIgnoreCase("ON");
        int white = jsonDoc["White"];
        
        int element = 0;    //0: MQTT_LED1_STATE 1: MQTT_LED2_STATE
        if (topicString.equals(MQTT_LED2_STATE)){
            element =1;
        }

        model->eep.elements.lightv[element].color = newval.color;
        
        if (white > 0) {
            model->eep.elements.lightv[element].intense = 0;
            model->eep.elements.lightv[element].white = white;
        } else {
            model->eep.elements.lightv[element].intense = newval.intense;
            model->eep.elements.lightv[element].white = newval.white;
        }
        // power off if powerstate is off
        if (!power) model->eep.elements.lightv[element].white = 0;        
    }
}

LightValues MqttClient::hsbStrToLightval(const char* hsbStr){
    //vonverts a string like "231,64,0" to LightVal
    char *ptr = NULL;
    ptr = strtok((char *)hsbStr, ",");  // takes a list of delimiters
    int h = String(ptr).toInt();
    ptr = strtok(NULL, ",");  
    int s = String(ptr).toInt();
    ptr = strtok(NULL, ",");  
    int b = String(ptr).toInt();
    //debug_print("new Values H: ");
    //debug_print(h);
    //debug_print(" S: ");
    //debug_print(s);       
    //debug_print(" B: ");
    //debug_println(b);
    LightValues retval;
    retval.color = h;
    retval.intense = s;
    retval.white = b;

    return retval;
}

void MqttClient::sendData(){
    static LightValues oldLight0 = model->eep.elements.lightv[0];
    static LightValues oldLight1 = model->eep.elements.lightv[1];
    unsigned long currentMs = millis();
    static unsigned long lastMs = 0;
    if ((model->mqttOnline && mqtt_client->connected()) && (currentMs - lastMs > 200) || (currentMs < lastMs)){
        lastMs = currentMs;
        
        if (model->eep.elements.lightv[0].white != oldLight0.white || model->eep.elements.lightv[0].color != oldLight0.color || model->eep.elements.lightv[0].intense != oldLight0.intense){
            //debug_println("send MQTT data Light 1");
            oldLight0 = model->eep.elements.lightv[0];
            String mqtt_colval = "", mqtt_whval = "";
            mqtt_colval += oldLight0.color;
            mqtt_colval += ", ";
            mqtt_colval += oldLight0.intense;
            mqtt_colval += ", ";
            mqtt_colval += oldLight0.white;
            mqtt_whval += oldLight0.white;
            //debug_println(mqtt_whval.c_str());
            if (oldLight0.intense > 10){
                mqtt_client->publish(MQTT_LED1_COLOR, mqtt_colval.c_str());
            } else {
                mqtt_client->publish(MQTT_LED1_WHITE, mqtt_whval.c_str());
            }
            model->lastSendTime = currentMs;
        }
        if (model->eep.elements.lightv[1].white != oldLight1.white || model->eep.elements.lightv[1].color != oldLight1.color || model->eep.elements.lightv[1].intense != oldLight1.intense){
            //debug_println("send MQTT data Light 2");
            oldLight1 = model->eep.elements.lightv[1];
            String mqtt_colval = "", mqtt_whval = "";
            mqtt_colval += oldLight1.color;
            mqtt_colval += ", ";
            mqtt_colval += oldLight1.intense;
            mqtt_colval += ", ";
            mqtt_colval += oldLight1.white;
            mqtt_whval += oldLight1.white;
            //debug_println(mqtt_whval.c_str());
            if (oldLight1.intense > 10){
                mqtt_client->publish(MQTT_LED2_COLOR, mqtt_colval.c_str());
            } else {
                mqtt_client->publish(MQTT_LED2_WHITE, mqtt_whval.c_str());
            }
            model->lastSendTime = currentMs;
        } 
    }
}