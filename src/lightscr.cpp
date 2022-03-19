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
    
}

void LightScreen::setDataModel(DataModel *m){
    model = m;  
    _modelisvalid = true; 
    lightval = model->eep.elements.lightv[model->lightscrnr]; //get data from model
    slider1->setValue(lightval.white);
    slider2->setValue(lightval.color);
    slider3->setValue(lightval.intense);
}

void LightScreen::update(){
    checkButtons();
    slider1->update();
    slider2->update();
    slider3->update();
    // check if the light values in the model changed
    static int oldWhite[2] = {0,0};
    static int oldIntense[2] = {0,0};
    static int oldColor[2] = {0,0};
    if (oldWhite[model->lightscrnr] != model->eep.elements.lightv[model->lightscrnr].white){
        //debug_println("value White chaged in model");
        slider1->setValue(model->eep.elements.lightv[model->lightscrnr].white);
        oldWhite[model->lightscrnr] = model->eep.elements.lightv[model->lightscrnr].white; 
    }
    if (oldColor[model->lightscrnr] != model->eep.elements.lightv[model->lightscrnr].color){
        //debug_println("value White chaged in model");
        slider2->setColor(model->eep.elements.lightv[model->lightscrnr].color);
        oldColor[model->lightscrnr] = model->eep.elements.lightv[model->lightscrnr].color; 
    }
    if (oldIntense[model->lightscrnr] != model->eep.elements.lightv[model->lightscrnr].intense){
        //debug_println("value Intense chaged in model");
        slider3->setValue(model->eep.elements.lightv[model->lightscrnr].intense);
        oldIntense[model->lightscrnr] = model->eep.elements.lightv[model->lightscrnr].intense; 
    }
}

void LightScreen::checkButtons(){
    static bool changehappend = false;
    if (_isactive){
        if (b_ceiling->wasPressed()){
            changehappend = true;
            model->lightscrnr = 0;
            lightval = model->eep.elements.lightv[0];
        }
        if (b_cupboard->wasPressed()){
            changehappend = true;
            model->lightscrnr = 1;
            lightval = model->eep.elements.lightv[1];
        }
        if (changehappend){
            changehappend = false;
            slider1->setValue(lightval.white);
            slider2->setValue(lightval.color);
            slider3->setValue(lightval.intense);
            drawScreen(true);
        }
    }
}

void LightScreen::updateData(){
    if (_modelisvalid){
        lightval.white = slider1->getValue();
        lightval.color = slider2->getColor();
        lightval.intense = slider3->getValue();
        model->eep.elements.lightv[model->lightscrnr] = lightval;
        model->eepromValid = false; //write to EEProm soon
    }
}

void LightScreen::handleDragEvent(int fromX, int fromY, int toX, int toY){
    slider1->handleDragEvent(fromX, fromY, toX, toY);
    slider2->handleDragEvent(fromX, fromY, toX, toY);
    slider3->handleDragEvent(fromX, fromY, toX, toY);
    updateData();
}

void LightScreen::handlePressEvent(int x, int y){
    slider1->handlePressEvent(x,y);
    slider2->handlePressEvent(x,y);
    slider3->handlePressEvent(x,y);
    updateData();
}


void LightScreen::drawScreen(bool draw){
    _isactive = draw;
    if (_modelisvalid){
        if (draw){
            if (model->lightscrnr == 0){
                b_ceiling->off.bg = TFT_YELLOW;
                b_ceiling->off.text = TFT_BLACK;
                b_ceiling->off.outline = TFT_YELLOW;
                b_cupboard->off.bg = TFT_BLACK;
                b_cupboard->off.text = TFT_YELLOW;
                b_cupboard->off.outline = TFT_YELLOW;
                //b_cupboard = true;
            } else {
                b_ceiling->off.bg = TFT_BLACK;
                b_ceiling->off.text = TFT_YELLOW;
                b_ceiling->off.outline = TFT_YELLOW;
                b_cupboard->off.bg = TFT_YELLOW;
                b_cupboard->off.text = TFT_BLACK;
                b_cupboard->off.outline = TFT_YELLOW;
            }

            M5.Lcd.setTextDatum(TL_DATUM);
            M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
            M5.Lcd.drawString("Helligkeit", 5, 33);
            M5.Lcd.drawString("Farbe", 5, 93);
            M5.Lcd.drawString("Intensitaet", 5, 153);
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
}
