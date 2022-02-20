/*
    Software for SmartWatch by olliy78

*/

//includes
#include "hmi.hpp"


SzeneScreen::SzeneScreen(){

}

SzeneScreen::~SzeneScreen(){

}

void SzeneScreen::init(){

    ButtonColors on_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    ButtonColors off_clrs = {TFT_BLACK, TFT_YELLOW, TFT_YELLOW};

    b_on  = new Button(20, 0, 230, 50, false, "EIN", off_clrs, on_clrs, CC_DATUM);
    b_szene1  = new Button(20, 60, 110, 50, false, "Szene 1", off_clrs, on_clrs, CC_DATUM);
    b_szene2  = new Button(135, 60, 110, 50, false, "Szene 2", off_clrs, on_clrs, CC_DATUM);
    b_off  = new Button(20, 120, 230, 50, false, "AUS", off_clrs, on_clrs, CC_DATUM);
    //slider = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 20, 170, 230);
    slider = new SmartSlider(SL_VERTICAL, SL_SIMPLE, 255, 0, 230);

    slider->setValue(50);    
}

void SzeneScreen::setDataModel(DataModel *m){
    model = m;
}

void SzeneScreen::update(){
    checkButtons();
    slider->update();
}

void SzeneScreen::checkButtons(){
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

void SzeneScreen::handleDragEvent(int fromX, int fromY, int toX, int toY){
    slider->handleDragEvent(fromX, fromY, toX, toY);
}

void SzeneScreen::handlePressEvent(int x, int y){
    slider->handlePressEvent(x,y);
}

void SzeneScreen::drawScreen(bool draw){
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
