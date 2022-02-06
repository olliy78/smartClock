/*
    Software for SmartWatch by olliy78

*/


#include "main.hpp"
#include "slider.hpp"

//HMI specific definitions
#define SLIDERH 40
#define SLIDERW 50
#define SLIDER1X 250
#define SLIDER1Y 10
#define SLIDER1H 200


using namespace std;

class SmartClockHmi{
    //private
    int screen;
    
    uint8_t hh, mm, ss; 
    uint32_t targetTime;                    // for next 1 second timeout
    bool AlarmState;
    //button objects
    Button *alarmbtn, *tl, *bl, *tr, *br, *slide1;
    Gesture *swipeLeft, *swipeRight;
    int slide1pos;

    SmartSlider *slider1, *slider2, *slider3;

    // private functions
    void checkButtons();
    void drawScreen();
    void showClock(int redraw);
    void doButtons();


public:
    SmartClockHmi();
    ~SmartClockHmi();
    void init();
    void update();
    void wipeScreen(int dir);
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
    
};
