/*
    Software for SmartWatch by olliy78

*/

//includes
#include "hmi.hpp"

AlarmScreen::AlarmScreen(){

}

AlarmScreen::~AlarmScreen(){

}

void AlarmScreen::init(){
    ButtonColors off_clrs = {TFT_YELLOW, TFT_BLACK, TFT_BLACK};
    ButtonColors on_clrs = {TFT_BLACK, TFT_YELLOW, TFT_YELLOW};
    ButtonColors r_off_clrs = {TFT_RED, TFT_BLACK, TFT_BLACK};
    ButtonColors r_on_clrs = {TFT_BLACK, TFT_RED, TFT_RED};
    b_al1  = new Button(0, 0, 160, 30, false, "Wecker 1", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
    b_al2  = new Button(160, 0, 160, 30, false, "Wecker 2", off_clrs, on_clrs, CC_DATUM, 0, 0, 2);
    b_m  = new Button(3, 180, 40, 40, false, "m", off_clrs, on_clrs, CC_DATUM);
    b_tu  = new Button(48, 180, 40, 40, false, "d", off_clrs, on_clrs, CC_DATUM);
    b_w  = new Button(93, 180, 40, 40, false, "m", off_clrs, on_clrs, CC_DATUM);
    b_th  = new Button(138, 180, 40, 40, false, "d", off_clrs, on_clrs, CC_DATUM);
    b_f  = new Button(183, 180, 40, 40, false, "f", off_clrs, on_clrs, CC_DATUM);
    b_sa  = new Button(228, 180, 40, 40, false, "s", r_off_clrs, r_on_clrs, CC_DATUM);
    b_su  = new Button(273, 180, 40, 40, false, "s", r_off_clrs, r_on_clrs, CC_DATUM);

    _hh = 12;
    _mm = 34;

}

void AlarmScreen::setDataModel(DataModel *m){
    model = m;
}

void AlarmScreen::update(){
    checkButtons();
}

void AlarmScreen::handleDragEvent(int fromX, int fromY, int toX, int toY){

}

void AlarmScreen::handlePressEvent(int x, int y){
    int xpos = 40;
    int ypos = 65; // Top left corner ot clock text, about half way down

    if (_isactive){
        //check positions of arrows to set the alarm time
        if (x >= xpos && x <= xpos+54 && y >= ypos-25 && y <= ypos+10) _hh = _hh + 10;
        if (x >= xpos && x <= xpos+54 && y >= ypos+75 && y <= ypos+100) _hh = _hh - 10;
        if (x >= xpos+55 && x <= xpos+110 && y >= ypos-25 && y <= ypos+10) _hh++;
        if (x >= xpos+55 && x <= xpos+110 && y >= ypos+75 && y <= ypos+100) _hh--;
        if (x >= xpos+120 && x <= xpos+174 && y >= ypos-25 && y <= ypos+10) _mm = _mm + 10;
        if (x >= xpos+120 && x <= xpos+174 && y >= ypos+75 && y <= ypos+100) _mm = _mm - 10;
        if (x >= xpos+175 && x <= xpos+229 && y >= ypos-25 && y <= ypos+10) _mm++;
        if (x >= xpos+175 && x <= xpos+229 && y >= ypos+75 && y <= ypos+100) _mm--;
        
        //handling under- and overflow
        if (_hh >= 24) _hh = 0;
        if (_hh < 0) _hh = 23;
        if (_mm >= 60) _mm = 0;
        if (_mm < 0) _mm =59;
        showClock();
    }
}

void AlarmScreen::drawScreen(bool draw){
    
    _isactive = draw;
    if (draw){
        showClock();
        b_al1->draw();
        b_al2->draw();
        b_m->draw();
        b_tu->draw();
        b_w->draw();
        b_th->draw();
        b_f->draw();
        b_sa->draw();
        b_su->draw();
    } else {
        b_al1->hide();
        b_al2->hide();
        b_m->hide();
        b_tu->hide();
        b_w->hide();
        b_th->hide();
        b_f->hide();
        b_sa->hide();
        b_su->hide();
    }

}

void AlarmScreen::checkButtons(){

    if (b_m->wasPressed()){

    }

}

void AlarmScreen::showClock() {
    if (_isactive){
        M5.Lcd.setTextDatum(TL_DATUM);
        int xpos = 40;
        int ypos = 65; // Top left corner ot clock text, about half way down
        // Draw hours and minutes
        M5.Lcd.setTextDatum(TL_DATUM);
        M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
        //M5.Lcd.setTextFont(7);
        //M5.Lcd.setTextSize(2);
        //M5.Lcd.drawString("12:34:56", xpos, ypos);
        //M5.Lcd.drawNumber(12, xpos, ypos);
        

        M5.Lcd.fillTriangle(xpos+10, ypos-5, xpos+40, ypos-5, xpos + 25, ypos-25, TFT_YELLOW);
        M5.Lcd.fillTriangle(xpos+65, ypos-5, xpos+95, ypos-5, xpos + 80, ypos-25, TFT_YELLOW);
        M5.Lcd.fillTriangle(xpos+10, ypos+83, xpos+40, ypos+83, xpos + 25, ypos+103, TFT_YELLOW);
        M5.Lcd.fillTriangle(xpos+65, ypos+83, xpos+95, ypos+83, xpos + 80, ypos+103, TFT_YELLOW);
        if (_hh < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
        xpos += M5.Lcd.drawNumber(_hh, xpos, ypos, 8);             // Draw hours
        xpos += M5.Lcd.drawChar(':', xpos, ypos - 8, 8);
        M5.Lcd.fillTriangle(xpos+10, ypos-5, xpos+40, ypos-5, xpos+25, ypos-25, TFT_YELLOW);
        M5.Lcd.fillTriangle(xpos+65, ypos-5, xpos+95, ypos-5, xpos+80, ypos-25, TFT_YELLOW);
        M5.Lcd.fillTriangle(xpos+10, ypos+83, xpos+40, ypos+83, xpos + 25, ypos+103, TFT_YELLOW);
        M5.Lcd.fillTriangle(xpos+65, ypos+83, xpos+95, ypos+83, xpos + 80, ypos+103, TFT_YELLOW);
        if (_mm < 10) xpos += M5.Lcd.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
        xpos += M5.Lcd.drawNumber(_mm, xpos, ypos, 8);             // Draw minutes
    }
}
