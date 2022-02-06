/*
    Software for SmartWatch by olliy78

*/

//includes
#include "slider.hpp"


SmartSlider::SmartSlider(){

}
SmartSlider::SmartSlider(int orientation, int style, int Xpos, int Ypos, int length){
    setup(orientation, style, Xpos, Ypos, length);
    ButtonColors on_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    _button  = new Button(_Xpos, _Ypos, SL_HIGHT, SL_WIDTH, false, "", on_clrs, on_clrs, TL_DATUM);
}

SmartSlider::~SmartSlider(){

}

void SmartSlider::setup(int orientation, int style, int Xpos, int Ypos, int length){

    _orientation = orientation;
    _style = style;
    _Xpos = Xpos;
    _Ypos = Ypos;
    _length = length;

}

//update is frquently called. it checks if something is happen and redraw the slider if necessary
void SmartSlider::update(){

}

//returns the value in range 0 ... 100
int SmartSlider::getValue(){
    return _sliderpos;
}

//set the value in range 0 ... 100 if the value changed, the element will redraw
void SmartSlider::setValue(int newval){
    //blank current position
    
    _sliderpos = newval;

    //draw Slider
    draw();
}

//if the slider is invisible, no graphical interaction will happen. After activation it will redraw. There is no blanking after deactivation.
void SmartSlider::setVisible(bool visible){
    _visible = visible;
    if (visible == true){
        draw();
    }
}

//returns the Zone where the slider is
Zone SmartSlider::getZone(){
    return Zone(_Xpos, _Ypos);
}

//slider may be redrawn after event
void SmartSlider::handleDragEvent(int fromX, int fromY, int toX, int toY){
    if (_visible){
        if (_orientation == SL_HIRIZONTAL){
            int pos = _sliderpos * ((_length - SL_HIGHT) * 0.01) + _Xpos;       //current slider position 
            Point fromPoint(fromX, fromY);
            //Point toPoint(toX, toY);
            Zone sliderZone(pos, _Ypos, SL_HIGHT, SL_WIDTH);
            if (sliderZone.contains(fromPoint)){
                Serial.println("Slider detected");
                _button->hide();
                M5.Lcd.fillRect(sliderZone.x, sliderZone.y, sliderZone.w, sliderZone.h, BLACK);
                int newPos = (toX - _Xpos) / ((_length - SL_HIGHT) * 0.01);
                if (newPos < 0) newPos = 0;
                if (newPos > 100) newPos = 100;
                _sliderpos = newPos;
                Serial.print("new position: ");
                Serial.println(_sliderpos);
                draw();
            }
        }
    }
}

//slider may be redrawn after event
void SmartSlider::handlePressEvent(int x, int y){
    if (_visible){
        Serial.println("Slider Tap");
        Point tapPoint(x, y);
        if (_orientation == SL_HIRIZONTAL){
            int pos = _sliderpos * ((_length - SL_HIGHT) * 0.01) + _Xpos;  //current slider position - needes to blank the screen
            Zone minusZ(_Xpos, _Ypos, SL_HIGHT, SL_WIDTH);
            Zone plusZ(_Xpos + _length - SL_HIGHT, _Ypos, SL_HIGHT, SL_WIDTH);
            if (minusZ.contains(tapPoint)){
                M5.Lcd.fillRect(pos, _Ypos, SL_HIGHT, SL_WIDTH, BLACK); //blank the screen
                _sliderpos--;
                draw();
            }
            else if (plusZ.contains(tapPoint)){
                M5.Lcd.fillRect(pos, _Ypos, SL_HIGHT, SL_WIDTH, BLACK); //blank the screen
                _sliderpos++;
                draw();
            }

        }
    }
}

// draw the slider
void SmartSlider::draw(){
    if (_visible){
        if (_orientation == SL_HIRIZONTAL){
            int pos = _sliderpos * ((_length - SL_HIGHT) * 0.01) + _Xpos;
            //M5.Lcd.drawRect(_Xpos, _Ypos, _length, SL_WIDTH, RED);
            M5.Lcd.drawRect(_Xpos, _Ypos + 17, _length, 16, SL_COLOR);
            M5.Lcd.fillTriangle(_Xpos + _length -16, _Ypos + 17, _Xpos + _length -16, _Ypos + 32, _Xpos + _length - 1, _Ypos + 25, SL_COLOR);
            M5.Lcd.fillTriangle(_Xpos + 16, _Ypos + 17, _Xpos + 16, _Ypos + 32, _Xpos + 1, _Ypos + 25, SL_COLOR);
            //_button->x = int(_Xpos + (_sliderpos * double(_length) * 0.01));
            _button->x = pos;
            _button->draw();

        }
        else if (_orientation == SL_VERTICAL){
            M5.Lcd.drawRect(_Xpos, _Ypos, SL_WIDTH, _length, RED);
        }
    }    
}