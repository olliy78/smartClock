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

    b_on  = new Button(20, 0, 280, 50, false, "EIN", off_clrs, on_clrs, CC_DATUM);
    b_szene1  = new Button(20, 60, 135, 50, false, "Szene 1", off_clrs, on_clrs, CC_DATUM);
    b_szene2  = new Button(160, 60, 135, 50, false, "Szene 2", off_clrs, on_clrs, CC_DATUM);
    b_off  = new Button(20, 120, 280, 50, false, "AUS", off_clrs, on_clrs, CC_DATUM);
    slider = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 20, 170, 280);

    slider->setValue(40);

}

void LightScreen::update(){
    M5.update();
    checkButtons();
    slider->update();
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
    slider->handleDragEvent(fromX, fromY, toX, toY);
}

void LightScreen::handlePressEvent(int x, int y){
    slider->handlePressEvent(x,y);
}


void LightScreen::drawScreen(bool draw){
       _isactive = draw;
    if (draw){
        b_off->draw();
        b_on->draw();
        b_szene1->draw();
        b_szene2->draw();
        slider->setVisible(true);
    } else {
        b_off->hide();
        b_on->hide();
        b_szene1->hide();
        b_szene2->hide();
        slider->setVisible(false);
    }
}
