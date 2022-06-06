#include <Arduino.h>
// #if defined(ESP32)
#include <WiFi.h>
// #elif defined(ESP8266)
  // #include <ESP8266WiFi.h>
// #endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include <addons/TokenHelper.h>
//Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>

#define WIFI_SSID "Hi"
#define WIFI_PASSWORD ""

#define API_KEY "AIzaSyC0go5OtqxkOornG3oQi83s9P-G-kwCG8I"

#define DATABASE_URL "https://hydroponics-d32d6-default-rtdb.asia-southeast1.firebasedatabase.app/" 

#define USER_EMAIL "USER_EMAIL"
#define USER_PASSWORD "USER_PASSWORD"

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0; 
float floatValue;
bool signupOK = false;