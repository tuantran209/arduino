/**
  ******************************************************************************

  File:	  ESP8266KeyPadPCF8574_LCDI2C.c
  Author: Tran Van Tuan
  Created on: 20/8/2023

  ******************************************************************************
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2); 
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <WiFiClient.h> 
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
WiFiManager wifiManager;

#define bt D5
char auth[] = "Your token";
const char *host = "192.168.152.162";  
static const int RXPin = D7, TXPin = D8; // Chân RX và TX của GPS
static const uint32_t GPSBaud = 9600;
static const int timeZone = 7; // Múi giờ số 7 (GMT+7)
static const double targetLatitude = 21.006617; // Vĩ độ của gateway
static const double targetLongitude =  105.828514; // Kinh độ của gateway

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

float buttonTimer = 0; //Timer for avoiding multiple keypresses at a time
bool buttonPressed = false; //status which keeps track if a key was pressed
String fullString = ""; //concatenated string
String pressedCharacter = ""; //recently pressed character on the keypad
bool stringAdded = false; //keeps track of the concatenation
double convertedNumber = 0; //final number. fullString converted into double

int mode = 0;
String ID_thietbi="1111111111111414";
double kinhdo =0.000001;
double vido = 0.000001;
double khoangcach = 0.00;
void setup()
{
  long t = millis();
  pinMode(bt, INPUT); //bt wire D5 and 3.3V
  EEPROM.begin(512);
  Serial.begin(9600); //Waitting 3s to join wipe wifi when press button
    //LCD
  lcd.init();                    
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("CongTyCoPhanGBS");
  delay(2500);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting.....");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected Wifi.....");
  lcd.setCursor(0, 1);
  lcd.print("192.168.4.1");
  delay(3000);
  if (digitalRead(bt) == 1) {
    wifiManager.resetSettings();
    lcd.setCursor(0, 1);
    lcd.print("192.168.4.1");
    delay(1000);
  }
  //  wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings from EEPROM on boot. 
  wifiManager.autoConnect("ESP8266");////if you get here you have connected to the WiFi
  Serial.println("connected...ok :)");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Done.....");
  lcd.setCursor(0, 1);
  lcd.print("D Connect Server");
  //-----------------
  //GPS
   ss.begin(GPSBaud);
  Wire.begin();
  Wire.setClock(400000L);
  Serial.begin(9600);
  Serial.println("PCF8574 with a keypad");
  //-----------------
  //----------------------------------------------------------------------------
  // lcd.print(fullString);
  // Serial.println(fullString);
}

void loop()
{
  readKeyPad(); //read the keypad
  checkInput(); //do something if a key was pressed
}


/******************************************************************************/
/*                     			 KeyPad LCD                             		      */
/******************************************************************************/
void checkInput()
{
  if (buttonPressed == true) //if a button was pressed...
  {
    if (stringAdded == true && mode == 0) //if a numberical button was pressed...
    {
      if(fullString.length() < 16) fullString = fullString + pressedCharacter; //concatenate
      //Print the string on the LCD
      if(fullString.length() == 16) ID_thietbi = fullString;
      lcd.setCursor(0, 1);
      lcd.print(fullString);
      stringAdded = false; //don't let the code enter this part again
    }

    if (pressedCharacter == "A") //if we pressed the A on the keypad...
    { 
      mode++;
      if(mode==3) mode=0;
      if(mode == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nhap ID:");
      lcd.setCursor(0,1);
      lcd.print(fullString);
      Serial.println(fullString);
      }
      if(mode == 1)
      {
      lcd.clear();
       Mode1();
       guiTinHieu();
      } 
      if(mode == 2){
      lcd.clear();
      Mode2();
      }
      delay(300); 
    }
    else if (pressedCharacter == "B"&&fullString.length() > 0 && mode == 0) //D = delete
    {
        fullString.remove(fullString.length() - 1);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nhap ID:");
        lcd.setCursor(0,1);
        lcd.print(fullString);
        Serial.println(fullString);
        delay(300);
    }
     else if (pressedCharacter == "C"&&fullString.length() > 0 && mode == 0) //D = delete
    {
        fullString = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nhap ID:");
        lcd.setCursor(0,1);
        lcd.print(fullString);
        Serial.println(fullString);
        delay(300);
    }
      else if (pressedCharacter == "D" && mode == 0) //D = delete
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Ket noi Server");
        //Ket noi vs server
        delay(1000);
        WiFi.mode(WIFI_OFF);        
        delay(1000);
        WiFi.mode(WIFI_STA);        
        WiFi.begin(WiFi.SSID(),WiFi.psk());    
        Serial.println("");
        Serial.print("Connecting");
        while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
        }
        Serial.println("");
        Serial.print("Connected to ");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP()); 
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Thanh Cong!!!");       
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nhap ID:");
        lcd.setCursor(0,1);
    }
  }

}

void readKeyPad()
{
  if (buttonPressed == true)
  {
    if (millis() - buttonTimer > 300)
    {
      //The button pressed is only set back to false after 300 ms, so we cannot press a button twice quickly
      buttonTimer = millis();
      buttonPressed = false;
    }
    else
    {
      //do nothing
    }
  }
  else
  {
    //B11101111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B11101111);
    //[P7]B11101111[P0] -> [P7]1110[P4] - activates first row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      //11101110 - P0 pin went low after pressing the button -> 1 was pressed
      case 238: //Button 1
        Serial.print("1");
        pressedCharacter = "1";
        buttonPressed = true;
        stringAdded = true;
        break;

      //11101101 - P1 pin went low after pressing the button -> 2 was pressed
      case 237: //Button 2
        Serial.print("2");
        pressedCharacter = "2";
        buttonPressed = true;
        stringAdded = true;
        break;

      //11101011 - P2 pin went low after pressing the button -> 3 was pressed
      case 235: //Button 3
        Serial.print("3");
        pressedCharacter = "3";
        buttonPressed = true;
        stringAdded = true;
        break;

      //11100111 - P3 pin went low after pressing the button -> A was pressed
      case 231: //Button A
        Serial.println("A");
        pressedCharacter = "A";
        buttonPressed = true;
        break;
    }
    //-------------------------------------------
    //B11011111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B11011111);
    //[P7]B11011111[P0] -> [P7]1101[P4] - activates second row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      //11011110 - P0 pin went low after pressing the button -> 2 was pressed
      case 222: //Button 4
        Serial.print("4");
        pressedCharacter = "4";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 221: //Button 5
        Serial.print("5");
        pressedCharacter = "5";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 219: //Button 6
        Serial.print("6");
        pressedCharacter = "6";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 215: //Button B
        Serial.println("B");
        pressedCharacter = "B";
        buttonPressed = true;
        break;
    }
    //-------------------------------------------
    //B10111111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B10111111);
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      case 190: //Button 7
        Serial.print("7");
        pressedCharacter = "7";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 189: //Button 8
        Serial.print("8");
        pressedCharacter = "8";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 187: //Button 9
        Serial.print("9");
        pressedCharacter = "9";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 183: //Button C
        Serial.println("C");
        pressedCharacter = "C";
        buttonPressed = true;
        break;
    }
    //-------------------------------------------
    //B01111111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B01111111);
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      case 126: //Button *
        Serial.print(".");
        pressedCharacter = ".";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 125: //Button 0
        Serial.print("0");
        pressedCharacter = "0";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 123: //Button #
        Serial.print("#");
        pressedCharacter = "#";
        buttonPressed = true;
        break;

      case 119: //Button D
        Serial.println("D");
        pressedCharacter = "D";
        buttonPressed = true;
        break;
    }
    buttonTimer = millis();
  }
}

void Mode1(){
    lcd.clear();
     while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      displayInfo_Vitri();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void Mode2(){
 while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      displayInfo_Khoangcanh_Time();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo_Vitri(){
    if (gps.location.isValid())
  {
    kinhdo = gps.location.lat();
    vido = gps.location.lng();
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("vido:");
    lcd.setCursor(5,0);
    lcd.print(gps.location.lat(), 6);
    lcd.setCursor(0,1);
    lcd.print("kinh:");
    lcd.setCursor(5,1);
    lcd.print(gps.location.lng(), 6);
  }
  else
  {
    //Serial.print(F("INVALID"));
      lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("vido:");
    lcd.setCursor(5,0);
    lcd.print(kinhdo, 6);
    lcd.setCursor(0,1);
    lcd.print("kinh:");
    lcd.setCursor(5,1);
    lcd.print(vido, 6);
  }
}

void displayInfo_Khoangcanh_Time(){
  if (gps.location.isValid())
  {
    lcd.setCursor(0,0);
    lcd.print( (TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), targetLatitude, targetLongitude))/1000,2);
    khoangcach = (TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), targetLatitude, targetLongitude))/1000;
  }
  else
  {
    //Serial.print(F("INVALID"));
    lcd.setCursor(0,0);
    lcd.print(khoangcach);
  }
  //time 
    if (gps.time.isValid())
  {
    int hour = gps.time.hour() + timeZone;
    if (hour < 0)
    {
      hour += 24;
    }
    else if (hour >= 24)
    {
      hour -= 24;
    }

    if (hour < 10){
        lcd.setCursor(8,0);
        lcd.print(0);
        lcd.setCursor(9,0);
        lcd.print(hour);
    }
    else
    {
      lcd.setCursor(8,0);
      lcd.print(hour);
    }
    lcd.setCursor(10,0);
    lcd.print(":");

    if (gps.time.minute() < 10)
      {
        lcd.setCursor(11,0);
        lcd.print(0);
        lcd.setCursor(12,0);
        lcd.print(gps.time.minute());
      }
    else{
      lcd.setCursor(11,0);
      lcd.print(gps.time.minute());
    }  
    lcd.setCursor(13,0);
    lcd.print(":");

    if (gps.time.second() < 10)
    {
      lcd.setCursor(14,0);
      lcd.print(0);
      lcd.setCursor(15,0);
      lcd.print(gps.time.second());
    } 
    else{
      lcd.setCursor(14,0);
      lcd.print(gps.time.second());
    }   
  }
  //hien thi duoc
  else
  {
    lcd.setCursor(8,0);
    lcd.print("INVALID");
  }
  //Date
  lcd.setCursor(0,1);
  lcd.print("DATE:");
  if (gps.date.isValid())
  { 
    if (gps.date.day() < 10)
    {
      lcd.setCursor(6,1);
      lcd.print(0);
      lcd.setCursor(7,1);
      lcd.print(gps.date.day());
    } 
    else{
      lcd.setCursor(6,1);
      lcd.print(gps.date.day());
    }   
    lcd.setCursor(8,1);
    lcd.print("/");

    if (gps.date.month() < 10)
    {
      lcd.setCursor(9,1);
      lcd.print(0);
      lcd.setCursor(10,1);
      lcd.print(gps.date.month());
    } 
    else{
      lcd.setCursor(9,1);
      lcd.print(gps.date.month());
    }   
    lcd.setCursor(11,1);
    lcd.print("/");

    lcd.setCursor(12,1);
    lcd.print(gps.date.year());

  }
  else
  {
    lcd.setCursor(6,1);
    lcd.print("INVALID");
  }
}

void guiTinHieu(){
  WiFiClient client;
  HTTPClient http;   
  String postData;
// Tạo chuỗi postData
 postData = String("mathietbi=") + ID_thietbi
   + String("&kinhdo=") + String(kinhdo, 6)
   + String("&vido=") + String(vido, 6)
   + String("&kc=") + String(khoangcach, 2);

  Serial.println(postData);
  http.begin(client,"http://192.168.152.162/demo/postdemo.php");             
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");   

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
}