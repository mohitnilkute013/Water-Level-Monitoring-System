// This example sends data to multiple variables to
// Ubidots through HTTP protocol.

/****************************************
 * Include Libraries
 ****************************************/

#include "Ubidots.h"
#include <LiquidCrystal_I2C.h>

/****************************************
 * Define Instances and Constants
 ****************************************/
 //Works for all ESP modules, Here pins are configured according to D1 mini module
int Sensor_pin_A = A0;  //Analog pin A0 of d1 mini
int Sensor_pin_D = D0;  //Digital pin D0 of d1 mini
int Buzzer_pin = D3;  //Digital pin D3 of d1 mini
String Device_Name = "Device 3";  // Set device name of your device according to your ubidots id
String Device_Variable = "Device3-v1";  //Set variable name according to your ubidots id

const char* UBIDOTS_TOKEN = "BBFF-UAXxUtHas9X4kOVBuoOWNsJQUTeUHF";  // Put here your Ubidots TOKEN
const char* WIFI_SSID = "Galaxy M31s85EE";      // Put here your Wi-Fi SSID
const char* WIFI_PASS = "qwert12345";      // Put here your Wi-Fi password

Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);
LiquidCrystal_I2C lcd(0x27, 16, 2);

/****************************************
 * Auxiliar Functions
 ****************************************/

// Put here your auxiliar functions

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  Serial.begin(115200);

  //----CONNECTING TO WIFI----//
  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  // ubidots.setDebug(true);  // Uncomment this line for printing debug messages

  //----SETTING PIN CONFIGURATIONS----//
  pinMode(Sensor_pin, INPUT);
  pinMode(Sensor_pin_D, INPUT);
  pinMode(Buzzer_pin, OUTPUT);
  

  //----LCD INITIALIZATION----//
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);         // move cursor to   (0, 0)
  lcd.print("Setup Completed");
  delay(1000);
}

void loop() {

  //----READING VALUE FROM SENSOR----//
  float water_value = analogRead(Sensor_pin); //analogRead(Sensor_pin)
  float water_value1 = digitalRead(Sensor_pin_D);
  
  Serial.print("Water level = ");
  Serial.print(water_value);
  Serial.print("  ");
  Serial.print(water_value1);
  
  water_value = map(water_value, 350, 1024, 70, 0);

  Serial.print(water_value);
  Serial.println("%");
  digitalWrite(Buzzer_pin, water_value1);


  //----LCD INTERFACING----//
  lcd.clear();// clear previous values from screen
  lcd.setCursor(0,0);
  lcd.print(Device_Name);
  lcd.setCursor(0,1);
  lcd.print("Water Level:");
  lcd.print(int(water_value));
  lcd.print("%");

  //----UPDATING VARIABLE VALUE ON UBIDOTS----//
  ubidots.add(Device_Variable, water_value);  // Change for your variable name

  bool bufferSent = false;
  bufferSent = ubidots.send();  // Will send data to a device label that matches the device Id

  if (bufferSent) {
    // Do something if values were sent properly
    Serial.println("Values sent by the device");
  }
  
  delay(2000);
}
