/*
    Software for SmartWatch by olliy78

*/

//includes
#include "main.hpp"
#include "hmi.hpp"
#include "model.hpp"
#include "mqtt.hpp"
#include "alarm.hpp"


//global objects
//WiFiClient espClient;
//PubSubClient client(espClient);

//SmartClockHmi *hmi = new SmartClockHmi();       //global HMI Object
//DataModel *myModel = new DataModel();             //global Data Model
//MqttClient *mqttclnt = new MqttClient();
SmartClockHmi *hmi;       //global HMI Object
DataModel *myModel;             //global Data Model
MqttClient *mqttclnt;
WiFiClient espClient;
PubSubClient *mqtt_client;
AlarmClock *alarmclk;

void buttonDragged(Event& e) {
  hmi->handleDragEvent(e.from.x, e.from.y, e.to.x, e.to.y);
}

void buttonPressed(Event& e) {
  hmi->handlePressEvent(e.to.x, e.to.y);
}

void buttonPressing(Event& e) {
  hmi->handlePressingEvent(e.to.x, e.to.y, e.duration);
}

void buttonReleasing(Event& e) {
  hmi->handleReleasingEvent(e.to.x, e.to.y);
}

void mqttCallback(char* topic, byte* payload, unsigned int length){
  mqttclnt->callback(topic, payload, length);
}

void setup() {
  
  
  hmi = new SmartClockHmi();       //global HMI Object
  myModel = new DataModel();             //global Data Model
  mqttclnt = new MqttClient();
  mqtt_client = new PubSubClient(espClient);
  alarmclk = new AlarmClock();

  // put your setup code here, to run once:
  Serial.begin(115200);
  debug_println("Hello World!");
  M5.begin();
  if (!EEPROM.begin(BYTESINEEPROM)){  //Request storage of SIZE size(success return 1). 
    debug_println("\nFailed to initialise EEPROM!"); //Serial output format string
    delay(1000000);
  }
  DateTime.begin();

  M5.Axp.SetLed(0);     //switch off the internal green LED
  M5.Axp.SetSpkEnable(1);
  
  alarmclk->setDataModel(myModel);
  alarmclk->init();
  
  hmi->init();
  myModel->readEEProm();
  hmi->setDataModel(myModel);
  hmi->setAlarmClock(alarmclk);
  hmi->selectScreen(0);
  
  //M5.Buttons.addHandler(wipeScr, E_GESTURE);
  //M5.Buttons.addHandler(toggleColor, E_DBLTAP);
  //M5.Buttons.addHandler(buttonDragged, E_DRAGGED);
  //M5.Buttons.addHandler(buttonPressed, E_TOUCH + E_RELEASE);
  M5.Buttons.addHandler(buttonPressed, E_TOUCH);
  M5.Buttons.addHandler(buttonPressing, E_PRESSING);
  M5.Buttons.addHandler(buttonReleasing, E_RELEASE);
  M5.Buttons.addHandler(buttonDragged, E_MOVE + E_DRAGGED);
  //swipeDown.addHandler(wipeScr, E_GESTURE);

  mqttclnt->setMQTTClient(mqtt_client);
  mqttclnt->setDataModel(myModel);
  mqttclnt->init();
  mqtt_client->setCallback(&mqttCallback); //Sets the message callback function

  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  hmi->update();
  mqttclnt->update();
  myModel->update();
  alarmclk->update();

  unsigned long currentMs = millis();
  static unsigned long lastMs = 0;
  if((currentMs - lastMs > 1000) || (currentMs < lastMs)){
      //Serial.println("ping");
      lastMs = currentMs;
      myModel->saveEEProm();
      //debug_println(M5.Axp.GetBatteryLevel());
  }
  //if (swipeDown.wasDetected()) Serial.println("Swiped down!");

  //M5.Axp.DeepSleep(5000000);
  //M5.Axp.GetBatPower()
  
}

