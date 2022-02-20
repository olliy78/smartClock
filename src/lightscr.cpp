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
    ButtonColors off_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    ButtonColors on_clrs = {TFT_BLACK, TFT_YELLOW, TFT_YELLOW};
    b_ceiling  = new Button(0, 0, 160, 30, false, "Deckenlampe", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
    b_cupboard  = new Button(160, 0, 160, 30, false, "Schrank", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);

    slider1 = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 10, 50, 300);
    slider2 = new SmartSlider(SL_HIRIZONTAL, SL_COLORPICKER, 10, 110, 300);
    slider3 = new SmartSlider(SL_HIRIZONTAL, SL_SIMPLE, 10, 170, 300);
    slider1->setValue(20);
    slider2->setValue(40);
    slider3->setValue(60);
}

void LightScreen::setDataModel(DataModel *m){
    Serial.println("Model assigned");
    if (m == nullptr) Serial.println("Nullpointer");
    model = m;
}

void LightScreen::update(){
    updateData();
    checkButtons();
    slider1->update();
    slider2->update();
    slider3->update();
}

void LightScreen::checkButtons(){

}

void LightScreen::updateData(){

}

void LightScreen::handleDragEvent(int fromX, int fromY, int toX, int toY){
    slider1->handleDragEvent(fromX, fromY, toX, toY);
    slider2->handleDragEvent(fromX, fromY, toX, toY);
    slider3->handleDragEvent(fromX, fromY, toX, toY);
}

void LightScreen::handlePressEvent(int x, int y){
    slider1->handlePressEvent(x,y);
    slider2->handlePressEvent(x,y);
    slider3->handlePressEvent(x,y);
}


void LightScreen::drawScreen(bool draw){
    _isactive = draw;
    if (draw){
        //if (model->lightscrnr == 0){
            b_ceiling->off.bg = TFT_YELLOW;
            b_ceiling->off.text = TFT_BLACK;
            b_ceiling->off.outline = TFT_YELLOW;
            b_cupboard->off.bg = TFT_BLACK;
            b_cupboard->off.text = TFT_YELLOW;
            b_cupboard->off.outline = TFT_YELLOW;
/*            //b_cupboard = true;
        } else {
            b_ceiling->off.bg = TFT_BLACK;
            b_ceiling->off.text = TFT_YELLOW;
            b_cupboard->off.bg = TFT_YELLOW;
            b_cupboard->off.text = TFT_BLACK;
        }
*/
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
        M5.Lcd.drawString("Weiss", 5, 30);
        M5.Lcd.drawString("Farbe", 5, 90);
        M5.Lcd.drawString("Intensitaet", 5, 150);
        b_cupboard->draw();
        b_ceiling->draw();
        slider1->setVisible(true);
        slider2->setVisible(true);
        slider3->setVisible(true);
    } else {
        //hide all buttons
        b_cupboard->hide();
        b_ceiling->hide();
        slider1->setVisible(false);
        slider2->setVisible(false);
        slider3->setVisible(false);
    }    
}
