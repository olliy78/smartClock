/*
    Software for SmartWatch by olliy78

*/

//includes
#include "model.hpp"

DataModel::DataModel(){
    //some initializations
    EEPROM.begin(BYTESINEEPROM);
    alscrnr = 0;
    lightscrnr = 0;


}

DataModel::~DataModel(){

}

void DataModel::initializeModel(){
    Serial.println("initialize Data");

    for (int i=0; i<2; i++){
        eep.elements.lightv[i].white = 20;
        eep.elements.lightv[i].color = 30;
        eep.elements.lightv[i].intense = 40;
        eep.elements.alarmv[i].hh = 12;
        eep.elements.alarmv[i].mm = 34;
        for (int d=0; d<7; d++){
            eep.elements.alarmv[i].day[d] = true;    
        }
    }
}

void DataModel::readEEProm(){
    debug_println("Read EEPROM");
    EEPROM.begin(BYTESINEEPROM);
    for (uint16_t i=0; i<BYTESINEEPROM; i++){
        eep.bytes[i] = EEPROM.readByte(i);
        //debug_print(i);
        //debug_print(" r:");
        debug_printHex(EEPROM.readByte(i));
        debug_print(" ");
    }

    debug_print("sizeof(eep.elements): ");
    debug_println(sizeof(eep.elements));
    debug_print("eep.elements.serialnr: ");
    debug_println(eep.elements.serialnr);
    debug_print("eep.elements.shadowSerialnr: ");
    debug_println(eep.elements.shadowSerialnr);

    if (eep.elements.length == BYTESINEEPROM && (eep.elements.serialnr == eep.elements.shadowSerialnr)){
        debug_println("EEProm valide");
        eepromValid = true;
    } else {
        debug_println("EEProm invalide --> initialize model");
        initializeModel();
        eepromValid = false;
    }

}

void DataModel::saveEEProm(){
    if (eepromValid == false){
        debug_print("writing EEPROM ");
        debug_print(BYTESINEEPROM);
        debug_print(" Bytes ");
        eep.elements.length = BYTESINEEPROM;  //store the size inside Data
        for (uint16_t i=0; i<BYTESINEEPROM; i++){
            if(eep.bytes[i] != EEPROM.readByte(i)){
                EEPROM.writeByte(i, eep.bytes[i]);
                EEPROM.commit();
                debug_print(".");
            }
        }
        debug_println("done");
        eepromValid = true;
    }
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
