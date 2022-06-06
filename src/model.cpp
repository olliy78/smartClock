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

void DataModel::update(){

    unsigned long currentMs = millis();
    static unsigned long lastMs = 0;
    //Update Time < 1 second in model
    if ((currentMs - lastMs > 1000) || (currentMs < lastMs)){
        lastMs = currentMs;
        //update time in model from RTC
        M5.Rtc.GetTime(&RTCtime);  // Gets the time in the real-time clock.
        M5.Rtc.GetDate(&RTCDate);
        time.yyyy = RTCDate.Year;
        time.mon = RTCDate.Month;
        time.dd = RTCDate.Date;
        //time.dow = RTCDate.WeekDay;
        time.hh = RTCtime.Hours;
        time.mm = RTCtime.Minutes;
        time.ss = RTCtime.Seconds;
        //calculate the day of week
        static int oldDay = 0;              //oly runn ons per day
        if (oldDay != time.dd){
            oldDay = time.dd;
            time.dow = calcDayOfWeek(time.yyyy, time.mon, time.dd) -2;      //correction from Sun = 1, Sat = 7 to Mon = 0 Sun = 6
            if (time.dow == -1) time.dow = 6; 
        }
        //char timeStrbuff[64];
        //sprintf(timeStrbuff, "day %d %d/%02d/%02d %02d:%02d:%02d", time.dow, time.yyyy, time.mon, time.dd, time.hh, time.mm, time.ss);
        //debug_println(timeStrbuff);
    }
}

void DataModel::initializeModel(){
    Serial.println("initialize Data");

    for (int i=0; i<2; i++){
        eep.elements.serialnr = SERIAL_NR;
        eep.elements.shadowSerialnr = SERIAL_NR;
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
    debug_println("");
    int eepSize = sizeof(eep.elements);
    if (eepSize != BYTESINEEPROM){
        debug_print("EEProm Size mismatch!  BYTESINEEPROM = ");
        debug_print(BYTESINEEPROM);
        debug_print(" size: ");
        debug_println(eepSize);
    }

    
    if (eep.elements.length == BYTESINEEPROM && (eep.elements.serialnr == eep.elements.shadowSerialnr)){
        debug_println("EEProm valide");
        eepromValid = true;
    } else {
        debug_print("eep.elements.serialnr: ");
        debug_println(eep.elements.serialnr);
        debug_print("eep.elements.shadowSerialnr: ");
        debug_println(eep.elements.shadowSerialnr);
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
    while(size > 0){;
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

void DataModel::setRTC(time_t time){

    //debug_println(asctime(gmtime(&time)));

    RTCDate.Year=gmtime(&time)->tm_year + 1900;
    RTCDate.Month=gmtime(&time)->tm_mon + 1;
    RTCDate.Date=gmtime(&time)->tm_mday;
    RTCtime.Hours=gmtime(&time)->tm_hour;
    RTCtime.Minutes=gmtime(&time)->tm_min;
    RTCtime.Seconds=gmtime(&time)->tm_sec;

    M5.Rtc.SetTime(&RTCtime);
    M5.Rtc.SetDate(&RTCDate);
        
}

int DataModel::calcDayOfWeek(int y, int m, int d) {
    //based on http://forum.arduino.cc/index.php?topic=197253.msg1456592#msg1456592
    // Old mental arithmetic method for calculating day of week
    // adapted for Arduino, for years 2000~2099
    // returns 1 for Sunday, 2 for Monday, etc., up to 7 for Saturday
    // for "bad" dates (like Feb. 30), it returns 0
    y = y-2000; //convert 2022 to just 22
    if (y > 99) return 0; // we don't accept years after 2099
    // we take care of bad months later
    if (d < 1) return 0; // because there is no day 0
    int w = 6; // this is a magic number (y2k fix for this method)
    // one ordinary year is 52 weeks + 1 day left over
    // a leap year has one more day than that
    // we add in these "leftover" days
    w += (y + (y >> 2));
    // correction for Jan. and Feb. of leap year
    if (((y & 3) == 0) && (m <= 2)) w--;
    // add in "magic number" for month
    switch (m) {
        case 1:  if (d > 31) return 0; w += 1; break;
        case 2:  if (d > ((y & 3) ? 28 : 29)) return 0; w += 4; break;
        case 3:  if (d > 31) return 0; w += 4; break;
        case 4:  if (d > 30) return 0; break;
        case 5:  if (d > 31) return 0; w += 2; break;
        case 6:  if (d > 30) return 0; w += 5; break;
        case 7:  if (d > 31) return 0; break;
        case 8:  if (d > 31) return 0; w += 3; break;
        case 9:  if (d > 30) return 0; w += 6; break;
        case 10: if (d > 31) return 0; w += 1; break;
        case 11: if (d > 30) return 0; w += 4; break;
        case 12: if (d > 31) return 0; w += 6; break;
        default: return 0;
    }
    // then add day of month
    w += d;
    // there are only 7 days in a week, so we "cast out" sevens
    while (w > 7) w = (w >> 3) + (w & 7);
    return w;
}