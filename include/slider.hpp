/*
    Software for SmartWatch by olliy78

*/
#pragma once


#include "main.hpp"

#define SL_VERTICAL 0
#define SL_HIRIZONTAL 1
#define SL_SIMPLE 0
#define SL_COLORPICKER 1

#define SL_HIGHT 40
#define SL_WIDTH 40
#define SL_RADIUS 16
#define SL_COLOR YELLOW

using namespace std;

struct rgbcolor {
    uint8_t r, g, b;
};

class SmartSlider{
private:
    //private
    int _sliderpos, _orientation, _style, _Xpos, _Ypos, _length;
    bool _visible;
    rgbcolor _color;
    
    // private functions
    void draw();
    void updateRGB();
    
public:
    SmartSlider();
    SmartSlider(int orientation, int style, int Xpos, int Ypos, int length);
    ~SmartSlider();
    void setup(int orientation, int style, int Xpos, int Ypos, int length);
    void update();
    int getValue();
    int getColor();
    void setValue(int newval);
    void setVisible(bool visible);
    Zone getZone();
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
    
};
