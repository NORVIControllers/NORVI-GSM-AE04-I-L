/*
 * NORVI GSM-AE04-I-L FINAL
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>
#include "FS.h"
#include "SD.h"
#include "RTClib.h"
#include <WiFi.h>

#define ANALOG_PIN_0 36

#define INPUT1 27
#define INPUT2 34
#define INPUT3 35
#define INPUT4 14
#define INPUT5 13
#define INPUT6 5

#define OUTPUT1 12
#define OUTPUT2 2

#define RS485_RX 25
#define RS485_TX 26
#define RS485_FC 22

#define GSM_RX 33
#define GSM_TX 32
#define GSM_RESET 21

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

#define OLED_RESET     -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

Adafruit_ADS1115 ads1;
Adafruit_ADS1115 ads2;

const float Rshunt = 10.0; 
const float Gain = 20.0;   
const float CalibrationFactor = 2.014;


RTC_DS3231 rtc; 
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int analog_value = 0;
  
int readSwitch(){
  analog_value = analogRead(ANALOG_PIN_0);
  return analog_value; //Read analog
}
unsigned long int timer1 = 0;



void setup() {
  Serial.begin(115200);
  Serial.println("Hello");

  WiFi.mode(WIFI_MODE_APSTA);
  
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX); 
  Serial2.begin(115200, SERIAL_8N1, GSM_RX, GSM_TX);
   

    
  pinMode(RS485_FC, OUTPUT);
  digitalWrite(RS485_FC, HIGH);   

  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);   
  
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);

  pinMode(15, OUTPUT);
  digitalWrite(15,HIGH);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);

   Wire.begin(16,17);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  display.display();
  
  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS 1 .");
    while (1);
  }
   ads1.setGain(GAIN_ONE);
   
  if (!ads2.begin(0x49)) {
    Serial.println("Failed to initialize ADS 2 .");
    while (1); 
  }
  ads2.setGain(GAIN_ONE);
  
  wifi_test();
  delay(300);
  RTC_Check();
  delay(1000);
  SD_CHECK();
  delay(1000);

  
  Serial.println("Testing Modem");  
  timer1 = millis();
  Serial2.println("AT");
  while(millis()<timer1+10000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
  timer1 = millis();
  Serial2.println("AT+CPIN?");
  while(millis()<timer1+10000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
  timer1 = millis();
  Serial2.println("AT+GSN");
  while(millis()<timer1+10000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
  Serial.println("Testing Modem Done");
 
  digitalWrite(RS485_FC, HIGH);   

}

void loop() {
  int16_t adc0, adc1, adc2, adc3;

  Serial.println("*********************************"); 
  
   while (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }

  while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }

  Serial.print(digitalRead(INPUT1));
  Serial.print(digitalRead(INPUT2));
  Serial.print(digitalRead(INPUT3));
  Serial.print(digitalRead(INPUT4));
  Serial.print(digitalRead(INPUT5));
  Serial.print(digitalRead(INPUT6));
  Serial.println(""); 
  
  adc0 = ads1.readADC_SingleEnded(0);
  adc1 = ads1.readADC_SingleEnded(1);
  adc2 = ads1.readADC_SingleEnded(2);
  adc3 = ads1.readADC_SingleEnded(3);
 
  float voltage0 = adc0 * 4.096 / 32768.0;  
  float voltage1 = adc1 * 4.096 / 32768.0; 
  float voltage2 = adc2 * 4.096 / 32768.0;  
  float voltage3 = adc3 * 4.096 / 32768.0; 

  float current0 = voltage0 / (Rshunt * Gain);
  float current1 = voltage1 / (Rshunt * Gain);
  float current2 = voltage2 / (Rshunt * Gain);
  float current3 = voltage3 / (Rshunt * Gain);

  float A0 =current0 * 1000.0*CalibrationFactor;
  float A1 =current1 * 1000.0*CalibrationFactor;
  float A2 =current2 * 1000.0*CalibrationFactor;
  float A3 =current3 * 1000.0*CalibrationFactor;
  
  Serial.print("Vin 0 : "); Serial.print(A0); Serial.println("mA");
  Serial.print("Vin 1 : "); Serial.print(A1); Serial.println("mA");
  Serial.print("Vin 2 : "); Serial.print(A2); Serial.println("mA");
  Serial.print("Vin 3 : "); Serial.print(A3); Serial.println("mA");

  adc0 = ads2.readADC_SingleEnded(0);
  adc1 = ads2.readADC_SingleEnded(1);
  
  float voltage4 = adc0 * 4.096 / 32768.0;   
  float voltage5 = adc1 * 4.096 / 32768.0;   

  float current4 = voltage4 / (Rshunt * Gain);
  float current5 = voltage5 / (Rshunt * Gain);
  
  float A4 =current4 * 1000.0*CalibrationFactor;
  float A5 =current5 * 1000.0*CalibrationFactor;
  
  Serial.print("Vin 4 : "); Serial.print(A4); Serial.println("mA");
  Serial.print("Vin 5 : "); Serial.print(A5); Serial.println("mA");


  Serial.println(""); 
  Serial.print("Push button  ");
  Serial.println(readSwitch());
  Serial.println(""); 
  
  digitalWrite(OUTPUT1, HIGH);
  delay(150);
  digitalWrite(OUTPUT2, HIGH);
  delay(150);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, LOW);
   
 
  digitalWrite(RS485_FC, HIGH);                    
  delay(100);
  Serial1.println(F("RS485 01 SUCCESS"));    
  delay(100);                               
  digitalWrite(RS485_FC, LOW) ;                    
                                             
  delay(100);     
  
  while (Serial1.available()) {  
    char c = Serial1.read();     
    Serial.write(c);            
  }
 delay(100);
}


void displayTime(void) {
  DateTime now = rtc.now();
     
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);

  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  delay(1000);

}

void RTC_Check(){
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
  }
 else{
 if (rtc.lostPower()) {
  
    Serial.println("RTC lost power, lets set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    
  }

   
  int a=1;
  while(a<6)
  {
  displayTime();  
  a=a+1;
  }
 }
}
void SD_CHECK(){
  uint8_t cardType = SD.cardType();
  if(SD.begin(15))
 {
  Serial.println("Card Mount: success");
  Serial.print("Card Type: ");
  if(cardType == CARD_MMC){
      Serial.println("MMC");
  } 
  else if(cardType == CARD_SD){
      Serial.println("SDSC");
  } 
  else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
  } 
  else {
      Serial.println("Unknown");
  }
  int cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("Card Size: %lluMB\n", cardSize);
  }
  if(!SD.begin(15)) {
     Serial.println("NO SD card");            
  }
}

void wifi_test(){
  
String str_macAddress;
byte mac[6];

WiFi.macAddress(mac);

   str_macAddress = (String(mac[0] >> 4,  HEX) + String(mac[0] & 0x0F, HEX)) + (":") +
                   (String(mac[1] >> 4,  HEX) + String(mac[1] & 0x0F, HEX)) + (":") +
                   (String(mac[2] >> 4,  HEX) + String(mac[2] & 0x0F, HEX)) + (":") +
                   (String(mac[3] >> 4,  HEX) + String(mac[3] & 0x0F, HEX)) + (":") +
                   (String(mac[4] >> 4,  HEX) + String(mac[4] & 0x0F, HEX)) + (":") +
                   (String(mac[5] >> 4,  HEX) + String(mac[5] & 0x0F, HEX));
  str_macAddress.toUpperCase();
  
  Serial.println("MAC Address: " + str_macAddress);

  String ssid =  str_macAddress; 
  const char* password = "12345678";   

  Serial.println("Setting up the Wi-Fi Access Point...");
  WiFi.softAP(ssid.c_str(), password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  Serial.print("Access Point SSID: ");
  Serial.println(ssid);

  Serial.println("Wi-Fi Access Point is active!");

}
