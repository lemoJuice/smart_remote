#include <GyverPortal.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <LinkedList.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
#include <GRGB.h>

#define RESET_RANGE_BUTTON 5
#define RESET_CLICK_BUTTON 5
#define DB_RECORD_LEN 107
#define BUTTON_PIN 15

struct LoginPass {
  uint8_t isFirstStart;
  char ssid[20] = " ";
  char pass[20] = " " ;
};
struct Database {
  char Device[16];
  char command_name[16];
  char command_code[16];
  decode_type_t protocol;//4 bytes
  uint16_t size;// 2 bytes
  uint8_t state[53];// 53 bytes
};

/* Soft AP network parameters */
const char * ap_ip = "1.1.1.1";
IPAddress apIP(1, 1, 1, 1);
IPAddress netMsk(255, 255, 255, 0);
/////////////////////////////////////

////DAATABASE///////////////////
LinkedList<Database> myList = LinkedList<Database>();
int record_count = 0;
short int mem_pos = 104;
LoginPass lp;
Database db;
/////////////////////////////////

////WEB//////////////////////
String main_ip;
GyverPortal captive;
GyverPortal ui;
bool isMain_start = 0;
String scanned;
//////////////////////////

//////////////IR///////////////////
const uint16_t kRecvPin = 14;
const uint16_t kIrLedPin = 16;
const uint16_t kCaptureBufferSize = 1024;  // 1024 == ~511 bits
const uint16_t kFrequency = 38000;
const uint8_t kTimeout = 50;
IRsend irsend(kIrLedPin);
IRrecv irrecv(kRecvPin, kCaptureBufferSize, kTimeout, false);
decode_results results;
bool isScan = false;
///RGB///////////////////////////
GRGB led(COMMON_ANODE, 5, 13, 12);
////////////////////////////////////
void setup() {
  delay(2000); 
  Serial.begin(9600);

  irrecv.enableIRIn();  // Start up the IR receiver.
  irsend.begin();       // Start up the IR sender
  EEPROM.begin(4096);
  //reset_wifi_settings();
  //digitalWrite(BUTTON_PIN, HIGH);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  led.enable();
  led.setRGB(255, 255, 255);
  led.setBrightness(100);
  Serial.println("read eeprom");
  
  
  EEPROM.get(0, lp);

  Serial.println("check val");
   Serial.println(lp.isFirstStart);
  if (lp.isFirstStart != 2){
    //start captive
    Serial.println("Start Captive");
    led.setRGB(0, 255, 0);
    led.setBrightness(100);
    loginPortal();
    lp.isFirstStart = 2;
    EEPROM.put(0, lp);
    EEPROM.commit();
    WiFi.softAPdisconnect();
    Serial.println("Start main portal");
    Serial.println(WiFi.status());
    start_mainPortal();
    led.setRGB(0, 0, 255);
    led.setBrightness(100);
  }else{
    reconnect_wifi();
    start_mainPortal();
    led.setRGB(0, 0, 255);
    led.setBrightness(100);
  }
}

void loop() {

  button(); 
  
  if (isMain_start == 1){
    ui.tick();  
  }

  getIRcommand();
 
}
