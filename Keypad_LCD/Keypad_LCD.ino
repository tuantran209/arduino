#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2); 
#include <Keypad.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3; // Chân RX và TX của Arduino
static const uint32_t GPSBaud = 9600;
static const int timeZone = 7; // Múi giờ số 7 (GMT+7)
static const double targetLatitude = 20.980462; // Vĩ độ của gateway
static const double targetLongitude = 105.787493; // Kinh độ của gateway
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// const byte ROWS = 4; //four rows
// const byte COLS = 4; //four columns
// char hexaKeys[ROWS][COLS] = {
//   {'1','2','3','A'},
//   {'4','5','6','B' },
//   {'7','8','9','C'},
//   {'*','0','#','D'}
// };
// byte rowPins[ROWS] =  {2,5,6,7}; //connect to the row pinouts of the keypad 
// byte colPins[COLS] =  {8,9,10,11};//connect to the column pinouts of the keypad
// //initialize an instance of class NewKeypad
// Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 
#define button_menu 'A'
#define button_del 'B'
#define button_re 'C'
int mode = 0;
String string_ID="";
void setup(){
  Serial.begin(9600);
  //GPS
   ss.begin(GPSBaud);
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
}
  
void loop(){  
    //xuLyMenu();
    if(mode==0) Mode0();
    if(mode==1) Mode1();
    if(mode==2) Mode2();
}

void Mode0() {
/*LCD hàng 1*/
  lcd.setCursor(0, 0);
  lcd.print("Nhap ID:");
  lcd.setCursor(0, 1); // Di chuyển con trỏ đến vị trí cuối chuỗi
  lcd.print(string_ID); // In ký tự mới
  Serial.println(string_ID);
  char c = customKeypad.getKey();
  
   if (c == button_menu) {
    mode++;
    lcd.clear();
    Mode1();
   }
  
  if (c != NO_KEY && c != 'A' && c != 'B' && c != 'C' && c != 'D') {
    if (string_ID.length() < 16) {
      string_ID += c;
    }
    lcd.setCursor(0, 1); // Di chuyển con trỏ đến vị trí cuối chuỗi
    lcd.print(string_ID); // In ký tự mới
    Serial.println(string_ID);
  }

    if (c == button_del) {
    if (string_ID.length() > 0) {
      string_ID.remove(string_ID.length() - 1); // Xóa ký tự cuối cùng trong chuỗi
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nhap ID:");
      lcd.setCursor(0, 1); 
      lcd.print(string_ID); 
      Serial.println(string_ID);
    }
  }
  
  if (c == button_re) {
    if (string_ID.length() > 0) {
      string_ID = "";
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Nhap ID:");
      lcd.setCursor(0, 1); 
      lcd.print(string_ID); 
      Serial.println(string_ID);
    }
  }
    
 }
//Hien thị tọa độ
void Mode1(){
  
  char c = customKeypad.getKey();
    if (c == button_menu) {
    mode++;
    lcd.clear();
    Mode2();
  } 
  //
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
  //
}
// Khoang cach
void Mode2(){
    //lcd.clear();
    char c = customKeypad.getKey();
    // lcd.setCursor(0,0);
    // lcd.print("Time:");
    if (c == button_menu) {
    mode = 0;
    lcd.clear();
    Mode0();
  } 
  //
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
  //
}

void displayInfo_Vitri(){
    if (gps.location.isValid())
  {
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
    lcd.setCursor(0,0);
    lcd.print(-1);
    lcd.setCursor(0,1);
    lcd.print(-1);
  }
}

void displayInfo_Khoangcanh_Time(){
  if (gps.location.isValid())
  {
    lcd.setCursor(0,0);
    lcd.print( (TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), targetLatitude, targetLongitude))/1000,2);
    
  }
  else
  {
    //Serial.print(F("INVALID"));
    lcd.setCursor(0,0);
    lcd.print("INVALID");
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