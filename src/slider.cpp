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
    draw();
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
                _button->hide();
                M5.Lcd.fillRect(sliderZone.x, sliderZone.y, sliderZone.w, sliderZone.h, BLACK);
                int newPos = (toX - _Xpos - SL_HIGHT * 0.5) / ((_length - SL_HIGHT) * 0.01);
                if (newPos < 0) newPos = 0;
                if (newPos > 100) newPos = 100;
                _sliderpos = newPos;
                Serial.println(_sliderpos);
                draw();
            }
        }
    }
}

//slider may be redrawn after event
void SmartSlider::handlePressEvent(int x, int y){
    if (_visible){
        if (_orientation == SL_HIRIZONTAL){
            if (x >= _Xpos && x <= (_Xpos + _length) && y >= _Ypos && y <= (_Ypos + SL_WIDTH)){
                int pos = _sliderpos * ((_length - SL_HIGHT) * 0.01) + _Xpos;  //current slider position - needes to blank the screen
                M5.Lcd.fillRect(pos, _Ypos, SL_HIGHT, SL_WIDTH, BLACK); //blank the screen
                int newPos = (x - _Xpos - SL_HIGHT * 0.5) / ((_length - SL_HIGHT) * 0.01);
                if (newPos < 0) newPos = 0;
                if (newPos > 100) newPos = 100;
                _sliderpos = newPos;
                draw();
            }
        }
    }
}

// draw the slider
void SmartSlider::draw(){
    if (_visible){
        if (_orientation == SL_HIRIZONTAL){
            int pos = _sliderpos * ((_length - SL_HIGHT) * 0.01) + _Xpos; // slider position on screen

            if (_style == SL_SIMPLE){
                int fillend = _sliderpos * (_length - SL_HIGHT) * 0.01 + (SL_HIGHT * 0.5);      //How far it is filled
                M5.Lcd.fillRoundRect(fillend, _Ypos + 17, _length, 16, 4, BLACK);
                M5.Lcd.fillRoundRect(_Xpos, _Ypos + 17, fillend, 16, 4, SL_COLOR);
                M5.Lcd.drawRoundRect(_Xpos, _Ypos + 17, _length, 16, 4, SL_COLOR);
                _button->x = pos;
                _button->draw();
            } else if (_style == SL_COLORPICKER){
                _button->x = pos;
                _button->draw();

                int seglen = (_length - 4) * 0.333;             // 
                double colScale = 255.0 / seglen;
                uint8_t r=0, g=0, b=0;
                for ( int segm = 0; segm < 3; segm++){
                    for (int x = 0; x < seglen; x++){
                        if (segm == 0){
                            r = 0;
                            g = x * colScale;
                            b = 255 - (x * colScale);
                        } else if (segm == 1){
                            r = x * colScale;
                            g = 255;
                            b = 0;
                        } else if (segm == 2){
                            r = 255;
                            g = 255 - (x * colScale);
                            b = 0;
                        } 
                        
                        uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
                        pos = _Xpos + (segm*seglen) + x + 3;
                        M5.Lcd.drawFastVLine(pos, _Ypos + 17, 16, color);
                    }
                }
                M5.Lcd.drawRoundRect(_Xpos, _Ypos + 17, _length, 16, 4 , SL_COLOR);

            }
            //int pos = _sliderpos * ((_length - SL_HIGHT) * 0.01) + _Xpos;
            //M5.Lcd.drawRect(_Xpos, _Ypos, _length, SL_WIDTH, RED);
            

            int fillend = _sliderpos * (_length - SL_HIGHT) * 0.01 + (SL_HIGHT * 0.5);
            //M5.Lcd.fillRoundRect(fillend, _Ypos + 17, _length, 16, 6, BLACK);
            //M5.Lcd.fillRoundRect(_Xpos, _Ypos + 17, fillend, 16, 6, RED);
            //M5.Lcd.drawRoundRect(_Xpos, _Ypos + 17, _length, 16, 6 , SL_COLOR);
            
            
        }
        else if (_orientation == SL_VERTICAL){
            M5.Lcd.drawRect(_Xpos, _Ypos, SL_WIDTH, _length, RED);
        }
    } else {
        //_button.
    }   
}