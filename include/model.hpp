/*
    Software for SmartWatch by olliy78

*/
#pragma once

#include <stdio.h>
#include "main.hpp"

 #define BYTESINEEPROM 8        //number ob bytes stored in EEPROM

struct LightValues {
    uint8_t white, color, intense;
};

struct AlarmValues {
    uint8_t hh, mm;
    bool day[7];
};

class DataModel {          //data model
    //private
    //helper functions

public:

    union crcdata{           //struct to store data in eeprom
      uint8_t bytes[11];         //byte elements to read and write byte-wise
      struct {
        char teststr[10]; 
        uint8_t number;
      }elem;
    }crcd; 

    union eepromdata{           //struct to store data in eeprom
        uint8_t bytes[BYTESINEEPROM];         //byte elements to read and write byte-wise
        struct {
            int length;                             //length of the EEPROM data block
            uint32_t serialnr;                       //serial number value must be the same as the shadowSerialnr
            uint8_t al1hh;                          //HH alarm 1
            uint8_t al1mm;                          //MM alarm 1
            uint8_t al2hh;                          //HH alarm 2
            uint8_t al2mm;                          //MM alarm 2
            LightValues lightv[2];
            AlarmValues alarmv[2];
            uint32_t shadowSerialnr;
        }elements;
    } eep;

    //temporary Data elements
    //to be initialized in constructor
    uint8_t alscrnr;        //which alarm is selected on alarm screen
    uint8_t lightscrnr;     //which Lamp is selected on light screen


    DataModel();
    ~DataModel();
    void initializeModel();
    void readEEProm();
    void saveEEProm();
    bool isModeValide(int mode, int port);
    uint16_t gen_crc16(uint8_t *data, uint16_t size);
};
