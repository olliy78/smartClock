/*
    Software for SmartWatch by olliy78

*/

//includes
#include "main.hpp"
#include "hmi.hpp"
#include "model.hpp"

//global objects
SmartClockHmi *hmi = new SmartClockHmi();       //global HMI Object
DataModel *model = new DataModel();             //global Data Model

void buttonDragged(Event& e) {
  hmi->handleDragEvent(e.from.x, e.from.y, e.to.x, e.to.y);
}

void buttonPressed(Event& e) {
  hmi->handlePressEvent(e.to.x, e.to.y);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.printf("Hello World!\n");
  M5.begin();
  hmi->init();
  hmi->setDataModel(model);
  hmi->selectScreen(0);
  
  //M5.Buttons.addHandler(wipeScr, E_GESTURE);
  //M5.Buttons.addHandler(toggleColor, E_DBLTAP);
  //M5.Buttons.addHandler(buttonDragged, E_DRAGGED);
  //M5.Buttons.addHandler(buttonPressed, E_TOUCH + E_RELEASE);
  M5.Buttons.addHandler(buttonPressed, E_TOUCH);
  M5.Buttons.addHandler(buttonDragged, E_MOVE + E_DRAGGED);
  //swipeDown.addHandler(wipeScr, E_GESTURE);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  hmi->update();
  unsigned long currentMs = millis();
    static unsigned long lastMs = 0;
    if((currentMs - lastMs > CYCLETIME) || (currentMs < lastMs)){
        //Serial.println("ping");
        lastMs = currentMs;
    }
  //if (swipeDown.wasDetected()) Serial.println("Swiped down!");
}

