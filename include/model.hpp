/*
    Software for SmartWatch by olliy78

*/
#pragma once

#include <stdio.h>
#include "main.hpp"

 #define BYTESINEEPROM 8        //number ob bytes stored in EEPROM

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
            
            uint32_t shadowSerialnr;
        }elements;
    } eep;

    DataModel();
    ~DataModel();
    void initializeModel();
    void readEEProm();
    void saveEEProm();
    bool isModeValide(int mode, int port);
    uint16_t gen_crc16(uint8_t *data, uint16_t size);
};
