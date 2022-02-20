/*
    Software for SmartWatch by olliy78

*/
#pragma once


#include "main.hpp"
#include "slider.hpp"
#include "model.hpp"

//HMI specific definitions
#define SLIDERH 40
#define SLIDERW 50
#define SLIDER1X 250
#define SLIDER1Y 10
#define SLIDER1H 200


using namespace std;


class Screen {

    
    

    protected:

    bool _isactive;
    void checkButtons();

    public:
    void init();
    void update();
    void drawScreen(bool draw);
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);

    
};

class MainScreen: public Screen {

    DataModel *model = nullptr;
    Button *alarmbtn;
    uint8_t _hh, _mm, _ss; 
    uint32_t targetTime;                    // for next 1 second timeout
    bool AlarmState;

    void showClock(int redraw);
    void checkButtons();

public:
    MainScreen();
    ~MainScreen();
    void init();
    void setDataModel(DataModel *m);
    void update();
    void drawScreen(bool draw);
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
};



class AlarmScreen: public Screen {

    DataModel *model = nullptr;
    int _hh, _mm;
    bool _isactive;
    Button *b_m, *b_tu, *b_w, *b_th, *b_f, *b_sa, *b_su, *b_al1, *b_al2;
    
    void showClock();
    void checkButtons();

    public:
    AlarmScreen();
    ~AlarmScreen();
    void init();
    void setDataModel(DataModel *m);
    void update();
    void drawScreen(bool draw);
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
};

class LightScreen: public Screen {

    DataModel *model = nullptr;
    bool _isactive;
    Button *b_ceiling, *b_cupboard;
    SmartSlider *slider1, *slider2, *slider3;
    
    
    
    void checkButtons();
    void updateData();

    public:
    LightScreen();
    ~LightScreen();
    void init();
    void setDataModel(DataModel *m);
    void update();
    void drawScreen(bool draw);
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
};

class SzeneScreen: public Screen {

    DataModel *model = nullptr;
    bool _isactive;
    Button *b_off, *b_on, *b_szene1, *b_szene2;
    SmartSlider *slider;
    
    
    void checkButtons();

    public:
    SzeneScreen();
    ~SzeneScreen();
    void init();
    void setDataModel(DataModel *m);
    void update();
    void drawScreen(bool draw);
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
};


class SmartClockHmi{
    //private
    DataModel *model = nullptr;
    int screen;
    

    //Screen Objects
    Screen *actscr;
    MainScreen *mainscr;
    AlarmScreen *alarmscr;
    LightScreen *lightscr;
    SzeneScreen *szenescr;

    //Gesture objects
    Gesture *swipeLeft, *swipeRight;

    SmartSlider *slider1, *slider2, *slider3;

    // private functions
    void checkButtons();
    void drawScreen();
    void doButtons();


public:
    SmartClockHmi();
    ~SmartClockHmi();
    void init();
    void setDataModel(DataModel *m);
    void update();
    void wipeScreen(int dir);
    void handleDragEvent(int fromX, int fromY, int toX, int toY);
    void handlePressEvent(int x, int y);
    
};

