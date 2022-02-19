/*
    Software for SmartWatch by olliy78

*/

//includes
#include "hmi.hpp"

//Gesture swipeLeft("swipe down", DIR_LEFT, 30);
//Gesture swipeRight("swipe down", DIR_RIGHT, 30);


//constructor
SmartClockHmi::SmartClockHmi(){    
    screen = 0;
}

//destructor
SmartClockHmi::~SmartClockHmi(){

}

//init HMI
void SmartClockHmi::init(){
    //Screens
    mainscr = new MainScreen();
    alarmscr = new AlarmScreen();
    lightscr = new LightScreen();
    szenescr = new SzeneScreen();
    mainscr->init();
    alarmscr->init();
    lightscr->init();
    szenescr->init();
    
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
        mainscr->update();
    } else if (screen == -1){
        lightscr->update();
    } else 
    if (screen == -2){
        szenescr->update();
    } else if (screen == 1){
        alarmscr->update();
    } else if (screen == 2){
        
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
}

void SmartClockHmi::handlePressEvent(int x, int y){
    mainscr->handlePressEvent(x, y);
    alarmscr->handlePressEvent(x,y);
    lightscr->handlePressEvent(x, y);
    szenescr->handlePressEvent(x, y);
}

void SmartClockHmi::handleDragEvent(int fromX, int fromY, int toX, int toY){    
    mainscr->handleDragEvent(fromX, fromY, toX, toY);
    alarmscr->handleDragEvent(fromX, fromY, toX, toY);
    lightscr->handleDragEvent(fromX, fromY, toX, toY);
    szenescr->handleDragEvent(fromX, fromY, toX, toY);
}

void SmartClockHmi::drawScreen(){
    //deactivate screens
    mainscr->drawScreen(false);
    alarmscr->drawScreen(false);
    lightscr->drawScreen(false);
    szenescr->drawScreen(false);

    //clear screen
    M5.Lcd.fillScreen(TFT_BLACK);
    // show buttons
    if (screen == 0){
        mainscr->drawScreen(true);
    } 
    else if (screen == -1) {
        lightscr->drawScreen(true);
    } 
    else if (screen == -2) {
        szenescr->drawScreen(true);
    }
    else  if (screen == 1){
        alarmscr->drawScreen(true);
    }
    //draw points for the selected screen
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

