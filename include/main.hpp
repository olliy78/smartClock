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

#define MQTT_SERVER "192.168.0.222"
#define MQTT_CLIENT_ID "M5Clock_Fine"
#define MQTT_STAT "stat/M5Clock_Fine/STAT"


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
