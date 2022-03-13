/*
    Software for SmartWatch by olliy78

*/

//includes
#include "main.hpp"
#include "hmi.hpp"
#include "model.hpp"
#include "mqtt.hpp"


//global objects
//WiFiClient espClient;
//PubSubClient client(espClient);

//SmartClockHmi *hmi = new SmartClockHmi();       //global HMI Object
//DataModel *myModel = new DataModel();             //global Data Model
//MqttClient *mqttclnt = new MqttClient();
SmartClockHmi *hmi;       //global HMI Object
DataModel *myModel;             //global Data Model
MqttClient *mqttclnt;

void buttonDragged(Event& e) {
  hmi->handleDragEvent(e.from.x, e.from.y, e.to.x, e.to.y);
}

void buttonPressed(Event& e) {
  hmi->handlePressEvent(e.to.x, e.to.y);
}

void setup() {
  hmi = new SmartClockHmi();       //global HMI Object
  myModel = new DataModel();             //global Data Model
  mqttclnt = new MqttClient();

  // put your setup code here, to run once:
  Serial.begin(115200);
  debug_println("Hello World!");
  M5.begin();
  if (!EEPROM.begin(BYTESINEEPROM)){  //Request storage of SIZE size(success return 1).  申请SIZE大小的存储(成功返回1)
    debug_println("\nFailed to initialise EEPROM!"); //串口输出格式化字符串.  Serial output format string
    delay(1000000);
  }
  hmi->init();
  myModel->readEEProm();
  hmi->setDataModel(myModel);
  hmi->selectScreen(0);
  
  //M5.Buttons.addHandler(wipeScr, E_GESTURE);
  //M5.Buttons.addHandler(toggleColor, E_DBLTAP);
  //M5.Buttons.addHandler(buttonDragged, E_DRAGGED);
  //M5.Buttons.addHandler(buttonPressed, E_TOUCH + E_RELEASE);
  M5.Buttons.addHandler(buttonPressed, E_TOUCH);
  M5.Buttons.addHandler(buttonDragged, E_MOVE + E_DRAGGED);
  //swipeDown.addHandler(wipeScr, E_GESTURE);

  mqttclnt->setDataModel(myModel);
  mqttclnt->init();

  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  hmi->update();
  mqttclnt->update();
  myModel->update();
  unsigned long currentMs = millis();
  static unsigned long lastMs = 0;
  if((currentMs - lastMs > 1000) || (currentMs < lastMs)){
      //Serial.println("ping");
      lastMs = currentMs;
      myModel->saveEEProm();
  }
  //if (swipeDown.wasDetected()) Serial.println("Swiped down!");
}

