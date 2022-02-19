/*
    Software for SmartWatch by olliy78

*/

//includes
#include "hmi.hpp"


LightScreen::LightScreen(){

}

LightScreen::~LightScreen(){

}

void LightScreen::init(){
    ButtonColors on_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    ButtonColors off_clrs = {TFT_BLACK, TFT_YELLOW, TFT_YELLOW};
    b_off  = new Button(0, 0, 100, 100, false, "b_off", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
    b_on  = new Button(0, 110, 100, 100, false, "b_on", off_clrs, on_clrs, CC_DATUM);
    b_szene1  = new Button(110, 0, 100, 100, false, "b_szene1", off_clrs, on_clrs, CC_DATUM);
    b_szene2  = new Button(110, 110, 100, 100, false, "b_szene2", off_clrs, on_clrs, CC_DATUM);

}

void LightScreen::update(){
    M5.update();
    checkButtons();
}

void LightScreen::checkButtons(){
    if (_isactive){
        if (b_off->wasPressed()){
            Serial.println("b_off");
        }
        if (b_on->wasPressed()){
            Serial.println("b_on");
        }
        if (b_szene1->wasPressed()){
            Serial.println("b_szene1");
        }
        if (b_szene2->wasPressed()){
            Serial.println("b_szene2");
        }
    }
}

void LightScreen::handleDragEvent(int fromX, int fromY, int toX, int toY){

}

void LightScreen::handlePressEvent(int x, int y){

}


void LightScreen::drawScreen(bool draw){
       _isactive = draw;
    if (draw){
        b_off->draw();
        b_on->draw();
        b_szene1->draw();
        b_szene2->draw();
    } else {
        b_off->hide();
        b_on->hide();
        b_szene1->hide();
        b_szene2->hide();
    }
}
