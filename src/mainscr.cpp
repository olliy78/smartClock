/*
    Software for SmartWatch by olliy78

*/

//includes
#include "hmi.hpp"

MainScreen::MainScreen(){
    AlarmState = false;
    _hh = 0, _mm = 0, _ss = 0; 
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

//update HMI
void MainScreen::update(){
    checkButtons();
    showClock(0);
}

void MainScreen::checkButtons() {
    if (_isactive){
        if (alarmbtn->wasPressed()){
            //Serial.println("Alarm");
            AlarmState = !AlarmState;
            if (AlarmState){
                alarmbtn->setLabel("Alarm ON");
                alarmbtn->off.bg = RED;
            } else {
                alarmbtn->setLabel("Alarm OFF");
                alarmbtn->off.bg = BLACK;
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
        showClock(1);
        alarmbtn->draw();
    } else {
        alarmbtn->hide();
    }
}

void MainScreen::showClock(int redraw) {
    static int omm = 99, oss = 99;
    static int xcolon = 0, xsecs = 0;

    if (redraw){
        omm = 99;
        oss = 99;
    }

    if (targetTime < millis()) {
        M5.Lcd.setTextDatum(TL_DATUM);
        // Set next update for 1 second later
        targetTime = millis() + 1000;
        // Adjust the time values by adding 1 second
        _ss++;              // Advance second
        if (_ss == 60) {    // Check for roll-over
        _ss = 0;          // Reset seconds to zero
        omm = _mm;        // Save last minute time for display update
        _mm++;            // Advance minute
            if (_mm > 59) {   // Check for roll-over
                _mm = 0;
                _hh++;          // Advance hour
                if (_hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
                _hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
                }
            }
        }
        // Update digital time
        int xpos = 0;
        int ypos = 10; // Top left corner ot clock text, about half way down
        int ysecs = ypos + 24;

        if (omm != _mm) { // Redraw hours and minutes time every minute
            omm = _mm;
            // Draw hours and minutes
            M5.Lcd.setTextDatum(TL_DATUM);
            M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
            //M5.Lcd.setTextFont(7);
            //M5.Lcd.setTextSize(2);
            //M5.Lcd.drawString("12:34:56", xpos, ypos);
            //M5.Lcd.drawNumber(12, xpos, ypos);
            if (_hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
            xpos += M5.Lcd.drawNumber(_hh, xpos, ypos, 8);             // Draw hours
            xcolon = xpos; // Save colon coord for later to flash on/off later
            xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
            if (_mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
            xpos += M5.Lcd.drawNumber(_mm, xpos, ypos, 8);             // Draw minutes
            xsecs = xpos; // Sae seconds 'x' position for later display updates
        }
        if (oss != _ss) { // Redraw seconds time every second
            oss = _ss;
            xpos = xsecs;
            //M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
            //M5.Lcd.setTextFont(7);
            xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
            //Draw seconds
            if (_ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6); // Add leading zero
            M5.Lcd.drawNumber(_ss, xpos, ysecs, 6);                     // Draw seconds
        }
    }
}
