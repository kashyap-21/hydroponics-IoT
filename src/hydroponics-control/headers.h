#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif

// Including the time library for generating the timestamps
//#include <ctime>

//#include "ctime"
#include <Firebase_ESP_Client.h>

// Provide the token generation process info.
#include <addons/TokenHelper.h>

/*edit starts */

// Including the library for taking the data form the DHT11 sensor( Humidity + Tempreture sensor)
#include "DHTesp.h"

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Hi"   // Provide appropreat Hotspot name
#define WIFI_PASSWORD "" // Provide the Password if there is no password then leave it blank

/* 2. Define the API Key */
#define API_KEY "AIzaSyC0go5OtqxkOornG3oQi83s9P-G-kwCG8I" // Firebase API key for the project

/* 2.1 Define database URL #####edited#####*/
#define DATABASE_URL "https://hydroponics-d32d6-default-rtdb.asia-southeast1.firebasedatabase.app/" // Database URL for RTDB

/* 3. Define the project ID */
#define FIREBASE_PROJECT_ID "hydroponics-d32d6"

/* 4. Define the user Email and password that alreadey registerd or added in your project */
#define USER_EMAIL "test@test.com"
#define USER_PASSWORD "superSecretPassword"

/*edit starts */

// Defination of required pins

#define DHTpin 15      // D15 of ESP32 DevKit
#define MoisturePin 33 // D33 of ESP32 DevKit
#define pHPin 32       // D32 of ESP32 DevKit
#define lumio 35
#define motorAcid 12
#define motorBase 14
#define motorMain 27
#define motorVentilation 26
#define lumino 16
#define channel 0 // DAC channel for PWM
#define freq 5000 // Frequency of DAC conversion
#define offset 8  // Bit-precision required (0-255)
