/*
    Software for SmartWatch by olliy78

*/

//includes
#include "hmi.hpp"

//Gesture swipeLeft("swipe down", DIR_LEFT, 30);
//Gesture swipeRight("swipe down", DIR_RIGHT, 30);


//constructor
SmartClockHmi::SmartClockHmi(){
    AlarmState = false;
    screen = -1;
    slide1pos = 0;
    hh = 0, mm = 0, ss = 0; 
    targetTime = 0;                    // for next 1 second timeout

}

//destructor
SmartClockHmi::~SmartClockHmi(){

}

//init HMI
void SmartClockHmi::init(){
    
    ButtonColors on_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    ButtonColors off_clrs = {TFT_BLACK, TFT_YELLOW, TFT_YELLOW};
    alarmbtn = new Button(0, 120, 320, 100, false ,"Alarm OFF", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
    tl  = new Button(0, 0, 100, 100, false, "top-left", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
    bl  = new Button(0, 110, 100, 100, false, "bottom-left", off_clrs, on_clrs, CC_DATUM);
    tr  = new Button(110, 0, 100, 100, false, "top-right", off_clrs, on_clrs, CC_DATUM);
    br  = new Button(110, 110, 100, 100, false, "bottom-right", off_clrs, on_clrs, CC_DATUM);

    slide1  = new Button(SLIDER1X, slide1pos + SLIDER1Y, SLIDERW, SLIDERH, false, "", on_clrs, on_clrs, CC_DATUM);


    swipeLeft = new Gesture("swipe left", 75, DIR_LEFT, 30, false, 500U);
    swipeRight = new Gesture("swipe right", 75, DIR_RIGHT, 30, false, 500U);

    drawScreen();
}

//update HMI
void SmartClockHmi::update(){
    M5.update();
    if (swipeRight->wasDetected()){
        wipeScreen(1);
    } 
    if (swipeLeft->wasDetected()){
        wipeScreen(-1);
    }
    checkButtons();

    if (screen == 0){
        showClock(0);
    }
}

void SmartClockHmi::wipeScreen(int dir){
    Serial.println("wipe");

    if (dir > 0) screen--;
    else screen ++;
    if (screen < MIN_SCREEN) screen = MIN_SCREEN;
    if (screen > MAX_SCREEN) screen = MAX_SCREEN;
    drawScreen();
}

void SmartClockHmi::checkButtons() {


    if (M5.BtnA.wasPressed()) {
        screen --;
        if (screen < MIN_SCREEN) screen = MIN_SCREEN;
        drawScreen();
    }
    if (M5.BtnB.wasPressed()) {
        screen = 0;
        drawScreen();
    }
    if (M5.BtnC.wasPressed()) {
        screen ++;
        if (screen > MAX_SCREEN) screen = MAX_SCREEN;
        drawScreen();
    }
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

    if (tl->wasPressed()){
        Serial.println("tl");
    }

    if (slide1->wasReleased()){
        Serial.println(slide1->y);

    }
  
}

void SmartClockHmi::handleDragEvent(int fromX, int fromY, int toX, int toY){    

    
    Point fromPoint(fromX, fromY);
    Point toPoint(toX, toY);
    Zone slider1zone(SLIDER1X, slide1pos + SLIDER1Y, SLIDERW, SLIDERH);

    if (slider1zone.contains(fromPoint)){
        Serial.println("Slider1 detected");
        slide1->hide();
        M5.Lcd.fillRect(SLIDER1X, slide1pos + SLIDER1Y, SLIDERW, SLIDERH, BLACK);
        M5.Lcd.drawRect(SLIDER1X +15, SLIDER1Y, 15, SLIDER1H, YELLOW);
        slide1pos = toY - SLIDER1Y;
        slide1->y = slide1pos + SLIDER1Y;
        slide1->draw();

        Serial.print("new position: ");
        Serial.println(slide1pos);
    }

    Serial.print(fromX);
    Serial.print(" ");
    Serial.print(fromY);
    Serial.print(" to ");
    Serial.print(toX);
    Serial.print(" ");
    Serial.println(toY);
}

void SmartClockHmi::drawScreen(){
        
    //hide all buttons
    alarmbtn->hide();
    tl->hide();
    bl->hide();
    tr->hide();
    br->hide();
    //clear screen
    M5.Lcd.fillScreen(TFT_BLACK);
    // show buttons
    if (screen == 0){
        alarmbtn->draw();
        showClock(1);
    } 
    else if (screen == -1) {
        tl->draw();
        bl->draw();
        tr->draw();
        br->draw();
        M5.Lcd.drawRect(SLIDER1X +15, SLIDER1Y, 15, SLIDER1H, YELLOW);
        slide1->draw();

    } 
    else {
        
    }

    if (screen == -2) M5.Lcd.fillCircle(120, 232, 5, TFT_WHITE);
    else M5.Lcd.drawCircle(120, 232, 5, TFT_WHITE);
    if (screen == -1) M5.Lcd.fillCircle(140, 232, 5, TFT_WHITE);
    else M5.Lcd.drawCircle(140, 232, 5, TFT_WHITE);
    if (screen == 0) M5.Lcd.fillCircle (160, 232, 5, TFT_WHITE);
    else M5.Lcd.drawCircle (160, 232, 5, TFT_WHITE);
    if (screen == 1) M5.Lcd.fillCircle(180, 232, 5, TFT_WHITE);
    else M5.Lcd.drawCircle(180, 232, 5, TFT_WHITE);
    if (screen == 2) M5.Lcd.fillCircle(200, 232, 5, TFT_WHITE);
    else M5.Lcd.drawCircle(200, 232, 5, TFT_WHITE);

}

void SmartClockHmi::showClock(int redraw) {
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
        ss++;              // Advance second
        if (ss == 60) {    // Check for roll-over
        ss = 0;          // Reset seconds to zero
        omm = mm;        // Save last minute time for display update
        mm++;            // Advance minute
            if (mm > 59) {   // Check for roll-over
                mm = 0;
                hh++;          // Advance hour
                if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
                hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
                }
            }
        }


        // Update digital time
        int xpos = 0;
        int ypos = 10; // Top left corner ot clock text, about half way down
        int ysecs = ypos + 24;

        if (omm != mm) { // Redraw hours and minutes time every minute
            omm = mm;
            // Draw hours and minutes
            M5.Lcd.setTextDatum(TL_DATUM);
            M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
            //M5.Lcd.setTextFont(7);
            //M5.Lcd.setTextSize(2);
            //M5.Lcd.drawString("12:34:56", xpos, ypos);
            //M5.Lcd.drawNumber(12, xpos, ypos);
            if (hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
            xpos += M5.Lcd.drawNumber(hh, xpos, ypos, 8);             // Draw hours
            xcolon = xpos; // Save colon coord for later to flash on/off later
            xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
            if (mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
            xpos += M5.Lcd.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
            xsecs = xpos; // Sae seconds 'x' position for later display updates
        }
        if (oss != ss) { // Redraw seconds time every second
            oss = ss;
            xpos = xsecs;
            //M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
            //M5.Lcd.setTextFont(7);
            xpos += M5.Lcd.drawChar(':', xsecs, ysecs, 6); // Seconds colon
            //Draw seconds
            if (ss < 10) xpos += M5.Lcd.drawChar('0', xpos, ysecs, 6); // Add leading zero
            M5.Lcd.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
        }
    }

}
