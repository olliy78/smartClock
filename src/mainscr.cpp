/*
    Software for SmartWatch by olliy78

*/

//includes
#include "hmi.hpp"

MainScreen::MainScreen(){
    AlarmState = false;
    targetTime = 0;                    // for next 1 second timeout
}

MainScreen::~MainScreen(){

}

//init HMI
void MainScreen::init(){

    //buttons
    ButtonColors on_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    ButtonColors off_clrs = {TFT_BLACK, TFT_YELLOW, TFT_YELLOW};
    alarmbtn = new Button(0, 120, 320, 100, false ,"Alarm OFF", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
}

void MainScreen::setDataModel(DataModel *m){
    model = m;
}

void MainScreen::setAlarmClock(AlarmClock *c){
    alarmclk = c;
}

//update HMI
void MainScreen::update(){
    if (_isactive){
        checkButtons();
        showClock(false);
        showStatus(false);
    }
}

void MainScreen::checkButtons() {
    if (_isactive){
        if (alarmbtn->wasPressed()){
            AlarmState = !AlarmState;
            if (AlarmState){
                alarmbtn->setLabel("Alarm ON");
                alarmbtn->off.bg = RED;
                alarmclk->alarmTone(true);
            } else {
                alarmbtn->setLabel("Alarm OFF");
                alarmbtn->off.bg = BLACK;
                alarmclk->alarmTone(false);
            }
        }
    }
}

void MainScreen::handlePressEvent(int x, int y){
    
}

void MainScreen::handleDragEvent(int fromX, int fromY, int toX, int toY){    
    
}

void MainScreen::drawScreen(bool draw){
     _isactive = draw;     
    if (draw){
        showClock(true);
        showStatus(true);
        alarmbtn->draw();
    } else {
        alarmbtn->hide();
    }
}

void MainScreen::showStatus(bool redraw){
    static bool oldstate = true;
    bool newstate = model->wifiOnline && model->mqttOnline;
    if ((oldstate != newstate) || redraw){
        if (newstate){
            M5.Lcd.fillRect(260,3,60,25, TFT_BLACK);   //Schrift entfernen
            redraw = true;
        } else {
            M5.Lcd.setTextDatum(TL_DATUM);
            M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
            M5.Lcd.drawString("offline!", 265,3,2);
        }
        oldstate = newstate;
    }
    //print accu level if not offline
    static int oldAccu = 0;
    int newAccu = (int)M5.Axp.GetBatteryLevel();
    if ((oldAccu != newAccu) || redraw){
        oldAccu = newAccu;
        if (newstate){
            M5.Lcd.fillRect(260,3,40,25, TFT_BLACK);   //Schrift entfernen
            M5.Lcd.drawNumber(newAccu, 265,3,2);
            M5.Lcd.drawString("%", 291,3,2);
            redraw = true;
        }
    }
    //print if accu is charging
    static bool oldCharge = false;
    bool newCharge = M5.Axp.isCharging();
    if ((oldCharge != newCharge) || redraw){
        oldCharge = newCharge;
        if (newstate){
            M5.Lcd.fillRect(300,3,20,25, TFT_BLACK);   //Schrift entfernen
            if (newCharge) M5.Lcd.drawString("+", 302,3,2);
            else M5.Lcd.drawString("-", 302,3,2);
            redraw = true;
        }
    }
}

void MainScreen::showClock(bool redraw) {
    static int omm = 99, oss = 99;

    //check if model is valid
    if (!(model->time.hh < 60 && model->time.hh >= 0 && model->time.mm < 60 && model->time.mm >= 0 && model->time.ss < 60 && model->time.ss >=0)){
        return;
    }

    if (redraw){
        omm = 99;
        oss = 99;
    }

    if (oss != model->time.ss) { // Redraw seconds time every second
        oss = model->time.ss;
        int xpos = 250;
        int ypos = 40;
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
        xpos += M5.Lcd.drawChar(':', xpos, ypos, 6); // Seconds colon
        //Draw seconds
        if (model->time.ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 6); // Add leading zero
        M5.Lcd.drawNumber(model->time.ss, xpos, ypos, 6);                     // Draw seconds
    }

    if (omm != model->time.mm) { // Redraw hours and minutes time every minute
        omm = model->time.mm;
        int xpos = 0;
        int ypos = 5;
        // Draw hours and minutes
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
        if (model->time.hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
        xpos += M5.Lcd.drawNumber(model->time.hh, xpos, ypos, 8);             // Draw hours
        xpos += M5.Lcd.drawChar(':', xpos, ypos, 8);
        if (model->time.mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
        xpos += M5.Lcd.drawNumber(model->time.mm, xpos, ypos, 8);             // Draw minutes
    }
}
