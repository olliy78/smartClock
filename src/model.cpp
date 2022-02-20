/*
    Software for SmartWatch by olliy78

*/

//includes
#include "model.hpp"

DataModel::DataModel(){
    Serial.println("Model created");
    //some initializations
    alscrnr = 0;
    lightscrnr = 0;

    //read from eeprom
    initializeModel();

}

DataModel::~DataModel(){

}

void DataModel::initializeModel(){
    Serial.println("initialize Data");
    eep.elements.al1hh = 12;
    eep.elements.al1mm = 34;
    eep.elements.al2hh = 21;
    eep.elements.al2mm = 43;

}

void DataModel::readEEProm(){

}

void DataModel::saveEEProm(){

}

bool DataModel::isModeValide(int mode, int port){

}

//crc16 checksum
#define CRC16 0x8005
uint16_t DataModel::gen_crc16(uint8_t *data, uint16_t size){
    uint16_t out = 0;
    int bits_read = 0, bit_flag;
    /* Sanity check: */
    if(data == NULL) return 0;
    while(size > 0){
        bit_flag = out >> 15;
        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits
        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7){
            bits_read = 0;
            data++;
            size--;
        }
        /* Cycle check: */
        if(bit_flag) out ^= CRC16;
    }
    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag) out ^= CRC16;
    }
    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }
    return crc;

}
