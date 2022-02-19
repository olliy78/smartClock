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
    mainscr->init();
    alarmscr = new AlarmScreen();
    alarmscr->init();

    //buttons
    ButtonColors on_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    ButtonColors off_clrs = {TFT_BLACK, TFT_YELLOW, TFT_YELLOW};
    tl  = new Button(0, 0, 100, 100, false, "top-left", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
    bl  = new Button(0, 110, 100, 100, false, "bottom-left", off_clrs, on_clrs, CC_DATUM);
    tr  = new Button(110, 0, 100, 100, false, "top-right", off_clrs, on_clrs, CC_DATUM);
    br  = new Button(110, 110, 100, 100, false, "bottom-right", off_clrs, on_clrs, CC_DATUM);

    slider1 = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 10, 10, 300);
    slider2 = new SmartSlider(SL_HIRIZONTAL, SL_COLORPICKER, 10, 80, 300);
    slider3 = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 10, 150, 300);

    swipeLeft = new Gesture("swipe left", 75, DIR_LEFT, 30, false, 500U);
    swipeRight = new Gesture("swipe right", 75, DIR_RIGHT, 30, false, 500U);

    slider1->setValue(20);
    slider2->setValue(40);
    slider3->setValue(60);
    
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
        
    } else 
    if (screen == -2){
        //update slider
        slider1->update();
        slider2->update();
        slider3->update();
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

    if (tl->wasPressed()){
        Serial.println("tl");
    }

}

void SmartClockHmi::handlePressEvent(int x, int y){
    slider1->handlePressEvent(x,y);
    slider2->handlePressEvent(x,y);
    slider3->handlePressEvent(x,y);
    alarmscr->handlePressEvent(x,y);
}

void SmartClockHmi::handleDragEvent(int fromX, int fromY, int toX, int toY){    
    slider1->handleDragEvent(fromX, fromY, toX, toY);
    slider2->handleDragEvent(fromX, fromY, toX, toY);
    slider3->handleDragEvent(fromX, fromY, toX, toY);
}

void SmartClockHmi::drawScreen(){
    //deactivate screens
    mainscr->drawScreen(false);
    alarmscr->drawScreen(false);

    //hide all buttons
    tl->hide();
    bl->hide();
    tr->hide();
    br->hide();
    slider1->setVisible(false);
    slider2->setVisible(false);
    slider3->setVisible(false);
    //clear screen
    M5.Lcd.fillScreen(TFT_BLACK);
    // show buttons
    if (screen == 0){
        mainscr->drawScreen(true);
    } 
    else if (screen == -1) {
        tl->draw();
        bl->draw();
        tr->draw();
        br->draw();
    } 
    else if (screen == -2) {
        slider1->setVisible(true);
        slider2->setVisible(true);
        slider3->setVisible(true);
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

