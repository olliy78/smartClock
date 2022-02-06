/*
    Software for SmartWatch by olliy78

*/


#include "main.hpp"

#define SL_VERTICAL 0
#define SL_HIRIZONTAL 1
#define SL_SIMPLE 0
#define SL_COLORPICKER 1

#define SL_HIGHT 40
#define SL_WIDTH 50
#define SL_COLOR YELLOW

using namespace std;

class SmartSlider{
private:
    //private
    int _sliderpos, _orientation, _style, _Xpos, _Ypos, _length;
    bool _visible;
    Button *_button;

    
    // private functions
    void draw();
    
public:
    SmartSlider();
    SmartSlider(int orientation, int style, int Xpos, int Ypos, int length);
    ~SmartSlider();
    void setup(int orientation, int style, int Xpos, int Ypos, int length);
    void update();
    int getValue();
    void setValue(int newval);
    void setVisible(bool visible);
    Zone getZone();
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
    
};
