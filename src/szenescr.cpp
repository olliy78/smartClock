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
    slider1 = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 10, 10, 300);
    slider2 = new SmartSlider(SL_HIRIZONTAL, SL_COLORPICKER, 10, 80, 300);
    slider3 = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 10, 150, 300);
    slider1->setValue(20);
    slider2->setValue(40);
    slider3->setValue(60);

}

void SzeneScreen::update(){
    M5.update();
    checkButtons();
    slider1->update();
    slider2->update();
    slider3->update();

}

void SzeneScreen::handleDragEvent(int fromX, int fromY, int toX, int toY){
    slider1->handleDragEvent(fromX, fromY, toX, toY);
    slider2->handleDragEvent(fromX, fromY, toX, toY);
    slider3->handleDragEvent(fromX, fromY, toX, toY);
}

void SzeneScreen::handlePressEvent(int x, int y){
    slider1->handlePressEvent(x,y);
    slider2->handlePressEvent(x,y);
    slider3->handlePressEvent(x,y);
}

void SzeneScreen::checkButtons(){

}

void SzeneScreen::drawScreen(bool draw){
    _isactive = draw;
    if (draw){
        slider1->setVisible(true);
        slider2->setVisible(true);
        slider3->setVisible(true);
    } else {
        //hide all buttons
        slider1->setVisible(false);
        slider2->setVisible(false);
        slider3->setVisible(false);
    }    
}
