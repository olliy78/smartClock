/*
    Software for SmartWatch by olliy78

*/
#pragma once


#include <Arduino.h>
#include <M5Core2.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <PubSubClientTools.h>
#include <ESPDateTime.h>
#include <time.h>
#include <ArduinoJson.h>



#define ROOM_1      //Fine
//#define ROOM_2      //Jonas
//#define ROOM_3      //Gaestezimmer

#define MQTT_SERVER "192.168.0.222"
#define MQTT_TIME_TOPPIC "stat/DateTime/RESULT"
#define MQTT_LED1_STATE "stat/DeckeGZ/RESULT"
#define MQTT_LED2_STATE "stat/SchrankGZ/RESULT"

#ifdef ROOM_1
    #define MQTT_LED1_COLOR "cmnd/DeckeFine/HSBColor"
    #define MQTT_LED1_WHITE "cmnd/DeckeFine/White"
    #define MQTT_LED2_COLOR "cmnd/SchrankFine/HSBColor"
    #define MQTT_LED2_WHITE "cmnd/SchrankFine/White"
    #define MQTT_CLIENT_ID "M5Clock_Fine"
    #define MQTT_STAT "stat/M5Clock_Fine/STAT"
    #define SERIAL_NR 123001
#endif
#ifdef ROOM_2
    #define MQTT_LED1_COLOR "cmnd/DeckeJonas/HSBColor"
    #define MQTT_LED1_WHITE "cmnd/DeckeJonas/White"
    #define MQTT_LED2_COLOR "cmnd/SchrankJonas/HSBColor"
    #define MQTT_LED2_WHITE "cmnd/SchrankJonas/White"
    #define MQTT_CLIENT_ID "M5Clock_Jonas"
    #define MQTT_STAT "stat/M5Clock_Jonas/STAT"
    #define SERIAL_NR 123002
#endif
#ifdef ROOM_3
    #define MQTT_LED1_COLOR "cmnd/DeckeGZ/HSBColor"
    #define MQTT_LED1_WHITE "cmnd/DeckeGZ/White"
    #define MQTT_LED2_COLOR "cmnd/SchrankGZ/HSBColor"
    #define MQTT_LED2_WHITE "cmnd/SchrankGZ/White"
    #define MQTT_CLIENT_ID "M5Clock_GZ"
    #define MQTT_STAT "stat/M5Clock_GZ/STAT"
    #define SERIAL_NR 123003
#endif


#define CYCLETIME 200               //cycle time for basic scheduler in ms default 10ms
#define MIN_SCREEN -2
#define MAX_SCREEN 2



//global funktions




//helper macros
//https://stackoverflow.com/questions/1644868/c-define-macro-for-debug-printing
#define DEBUG 1
#define debug_print(str) do { if (DEBUG){ Serial.print(str); } } while (0)
#define debug_println(str) do { if (DEBUG){ Serial.println(str); } } while (0)
#define debug_printHex(str) do { if (DEBUG){ Serial.print(str, HEX); } } while (0)
#define debug_printHexln(str) do { if (DEBUG){ Serial.println(str, HEX); } } while (0)
