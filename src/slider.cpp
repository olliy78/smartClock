/*
    Software for SmartWatch by olliy78

*/

//includes
#include "slider.hpp"


SmartSlider::SmartSlider(){

}
SmartSlider::SmartSlider(int orientation, int style, int Xpos, int Ypos, int length){
    setup(orientation, style, Xpos, Ypos, length);
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

rgbcolor SmartSlider::getColor(){
    return _color;
}

//set the value in range 0 ... 100 if the value changed, the element will redraw
void SmartSlider::setValue(int newval){
    
    //if visible blank current position and draw the new one
    if (_visible){
        if (_orientation == SL_HIRIZONTAL){
            int pos = _sliderpos * ((_length - SL_HIGHT) * 0.01) + _Xpos;       //current slider position 
            Zone sliderZone(pos, _Ypos, SL_HIGHT, SL_WIDTH);
            M5.Lcd.fillRect(sliderZone.x, sliderZone.y, sliderZone.w, sliderZone.h, BLACK);
        } else {
            int pos = (100 - _sliderpos) * ((_length - SL_HIGHT) * 0.01) + _Ypos;       //current slider position 
            Zone sliderZone(_Xpos, pos, SL_WIDTH, SL_HIGHT);
            M5.Lcd.fillRect(sliderZone.x, sliderZone.y, sliderZone.w, sliderZone.h, BLACK);
        }
        
    }
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
            Zone sliderZone(pos, _Ypos, SL_HIGHT, SL_WIDTH);
            if (sliderZone.contains(fromPoint)){
                M5.Lcd.fillRect(sliderZone.x, sliderZone.y, sliderZone.w, sliderZone.h, BLACK);
                int newPos = (toX - _Xpos - SL_HIGHT * 0.5) / ((_length - SL_HIGHT) * 0.01);
                if (newPos < 0) newPos = 0;
                if (newPos > 100) newPos = 100;
                _sliderpos = newPos;
                draw();
            }
        } else {
            int pos = (100 - _sliderpos) * ((_length - SL_HIGHT) * 0.01) + _Ypos;       //current slider position 
            Point fromPoint(fromX, fromY);
            Zone sliderZone(_Xpos, pos, SL_WIDTH, SL_HIGHT);
            if (sliderZone.contains(fromPoint)){
                M5.Lcd.fillRect(sliderZone.x, sliderZone.y, sliderZone.w, sliderZone.h, BLACK);
                int newPos = (toY - _Ypos - SL_HIGHT * 0.5) / ((_length - SL_HIGHT) * 0.01);
                if (newPos < 0) newPos = 0;
                if (newPos > 100) newPos = 100;
                _sliderpos = 100 - newPos;
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
        } else {
            if (x >= _Xpos && x <= (_Xpos + SL_WIDTH) && y >= _Ypos && y <= (_Ypos + _length)){
                int pos = (100 - _sliderpos) * ((_length - SL_HIGHT) * 0.01) + _Ypos;  //current slider Y-position - needes to blank the screen
                M5.Lcd.fillRect(_Xpos, pos, SL_WIDTH, SL_HIGHT, BLACK); //blank the screen
                int newPos = (y - _Ypos - SL_HIGHT * 0.5) / ((_length - SL_HIGHT) * 0.01);
                if (newPos < 0) newPos = 0;
                if (newPos > 100) newPos = 100;
                _sliderpos = 100 - newPos;
                draw();
            }
        }
    }
}

// draw the slider
void SmartSlider::draw(){
    if (_visible){
        if (_orientation == SL_HIRIZONTAL){
            //M5.Lcd.drawRect(_Xpos, _Ypos, _length, SL_WIDTH, RED);
            if (_style == SL_SIMPLE){
                int fillend = _sliderpos * (_length - 2*SL_RADIUS) * 0.01 + SL_RADIUS;      //How far it is filled
                M5.Lcd.fillRoundRect(fillend + _Xpos, _Ypos + 12, _length, 16, 4, BLACK);
                M5.Lcd.fillRoundRect(_Xpos, _Ypos + 12, fillend + _Xpos, 16, 4, SL_COLOR);
                M5.Lcd.drawRoundRect(_Xpos, _Ypos + 12, _length, 16, 4, SL_COLOR);
                M5.Lcd.fillCircle(fillend + _Xpos, _Ypos+20, SL_RADIUS, SL_COLOR);
            } else if (_style == SL_COLORPICKER){
                int seglen = (_length - 4) * 0.25;             // 
                int curpos = 0;
                double colScale = 255.0 / seglen;
                uint8_t r=0, g=0, b=0;
                for ( int segm = 0; segm < 4; segm++){
                    for (int x = 0; x < seglen; x++){
                        if (segm == 0){
                            r = 255 - (x * colScale);
                            g = 0;
                            b = x * colScale;
                        } else if (segm == 1){
                            r = 0;
                            g = x * colScale;
                            b = 255 - (x * colScale);
                        } else if (segm == 2){
                            r = x * colScale;
                            g = 255;
                            b = 0;
                        } else if (segm == 3){
                            r = 255;
                            g = 255 - (x * colScale);
                            b = 0;
                        } 
                        uint16_t color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
                        curpos = _Xpos + (segm*seglen) + x + 1;
                        M5.Lcd.drawFastVLine(curpos, _Ypos + 12, 16, color);
                    }
                }
                int pos = _sliderpos * (_length - 2*SL_RADIUS) * 0.01 + SL_RADIUS;      // slider position on screen
                updateRGB();
                uint16_t slidercol = ((_color.r & 0xF8) << 8) | ((_color.g & 0xFC) << 3) | (_color.b >> 3);
                M5.Lcd.drawRoundRect(_Xpos, _Ypos + 12, _length, 16, 4 , SL_COLOR);
                M5.Lcd.fillCircle(pos + _Xpos, _Ypos+20, SL_RADIUS, slidercol);
            }
        }
        else if (_orientation == SL_VERTICAL){
            //M5.Lcd.drawRect(_Xpos, _Ypos, SL_WIDTH, _length, RED);
            if (_style == SL_SIMPLE){
                int fillend = (100 - _sliderpos) * (_length - 2*SL_RADIUS) * 0.01 + SL_RADIUS;      //How far it is filled
                M5.Lcd.fillRoundRect(_Xpos+12, _Ypos, 16, fillend + _Ypos, 4, BLACK);
                M5.Lcd.fillRoundRect(_Xpos+12, fillend + _Ypos, 16, _length - fillend + _Ypos, 4, SL_COLOR);                
                M5.Lcd.drawRoundRect(_Xpos+12, _Ypos, 16, _length, 4, SL_COLOR);
                M5.Lcd.fillCircle(_Xpos+20, fillend + _Ypos, SL_RADIUS, SL_COLOR);
            } else if (_style == SL_COLORPICKER){

            }
        }
    }   
}

void SmartSlider::updateRGB(){
    if (_sliderpos >=0 && _sliderpos< 25){
        _color.r = 255 - (_sliderpos * 10.24);
        _color.g = 0;
        _color.b = _sliderpos * 10.24;
    } else if (_sliderpos >=25 && _sliderpos < 50){
        _color.r = 0;
        _color.g = (_sliderpos - 25) * 10.24;
        _color.b = 255 - ((_sliderpos-25) * 10.24);
    } else if (_sliderpos >=50 && _sliderpos < 75){
        _color.r = (_sliderpos - 50) * 10.24;
        _color.g = 255;
        _color.b = 0;
    } else if (_sliderpos >=75 && _sliderpos <= 100){
        _color.r = 255;
        _color.g = 255 - ((_sliderpos-75) * 10.2);
        _color.b = 0;
    }
}