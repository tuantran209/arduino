#define button_menu 2
#define button_up 3
#define button_down 4
#define button_OK 5
#define chanCoi 6
#define chanDen 7
#define chanQuat 8
#define chanLed 10
#include "DHT.h"            
const int DHTPIN = 9;      
const int DHTTYPE = DHT11;
DHT dht(DHTPIN,DHTTYPE);
#include <SoftwareSerial.h>
SoftwareSerial nodemcu(1, 0); //tx-1, rx-0
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include <Wire.h>

/* Địa chỉ của DS3231 */
const byte DS3231 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS3231 */
const byte NumberOfFields = 7;
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;
int mode = 0;
int ngay_no = 31,thang_no = 12,gio_no = 23 ,phut_no = 59,nhiet_do_no=30;
int statusmode1=0;
/*Khai báo biến nhiệt độ*/
int temp; 
/*Khai báo biến để thông báo trứng nở*/
unsigned long time;
bool coiBaoHieu = false;
/*Khai báo biến để gửi dữ liệu*/
int sdata1 = 0;
int sdata2 = 0;
int sdata3 = 0;
int sdata4 = 0;
int sdata5 = 0;
int sdata6 = 0;
int sdata7 = 0;
int sdata8 = 0;
int sdata9 = 0;
int sdata10 = 0;
String cdata;
void setup() {
  Wire.begin();
  /* cài đặt thời gian cho module */
  setTime(11, 17, 00, 4, 24, 5, 23);  //  11:17:00 T5 24-05-2023
  dht.begin();
  Serial.begin(9600);
  lcd.init();             
  lcd.backlight();
  pinMode(button_menu,INPUT);
  pinMode(button_up,INPUT);
  pinMode(button_down,INPUT);
  pinMode(button_OK,INPUT);
  pinMode(chanDen, OUTPUT);
  pinMode(chanQuat, OUTPUT);
  pinMode(chanCoi, OUTPUT);
  /*đèn báo hiệu dữ liệu truyền giữa Uno và ESP8266*/
  nodemcu.begin(9600);
  pinMode(chanLed, OUTPUT); 
  digitalWrite(chanLed, LOW); 
}
void loop() {
  /* Đọc dữ liệu của DS3231 */
  readDS3231();
  /* Đọc dữ liệu nhiệt độ DHT11 */
  temp = dht.readTemperature(); 
  guiTinHieu();
  /* Tạo menu gồm 3 mode hiển thị trên LCD */
  xuLyMenu();
  if(mode==0) Mode0();
  if(mode==1) Mode1();
  if(mode==2) Mode2();
  //Serial.print(digitalRead(button_OK));
  delay(1000);
}
void guiTinHieu(){
  temp = dht.readTemperature();
  sdata1 = gio_no;
  sdata2 = phut_no;
  sdata3 = ngay_no;
  sdata4 = thang_no;
  sdata5 = nhiet_do_no;
  sdata6 = hour;
  sdata7 = minute;
  sdata8 = day;
  sdata9 = month;
  sdata10 = temp;
  /*thông báo quá trình truyền*/
  digitalWrite(chanLed, HIGH);  
  delay(1000);               
  digitalWrite(chanLed, LOW);    
  delay(1000);  
  cdata = String(sdata1) + "," + String(sdata2) + "," + String(sdata3) + "," + String(sdata4) + "," + String(sdata5)+","+String(sdata6) + "," + String(sdata7) + "," + String(sdata8) + "," + String(sdata9);
  Serial.println(cdata);
  nodemcu.println(cdata);
  
  delay(2000); // 2 seconds
  cdata = "";
}
void xuLyMenu(){
  
  if (digitalRead(button_menu)==0) {
  if(mode = 2) mode=0;
    else mode++;
  }  
}
/*đọc giá trị từ cảm biến */
void readDS3231() {
  Wire.beginTransmission(DS3231);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS3231, NumberOfFields);

  second = bcd2dec(Wire.read() & 0x7f);
  minute = bcd2dec(Wire.read());
  hour = bcd2dec(Wire.read() & 0x3f);  // chế độ 24h.
  wday = bcd2dec(Wire.read());
  day = bcd2dec(Wire.read());
  month = bcd2dec(Wire.read());
  year = bcd2dec(Wire.read());
  year += 2000;
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num) {
  return ((num / 16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num) {
  return ((num / 10 * 16) + (num % 10));
}

void Mode0() {
/*LCD hàng 1*/
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(hour);
  lcd.setCursor(2,0);
  lcd.print(":");
  lcd.setCursor(3,0);
  lcd.print(minute);
  lcd.setCursor(6,0);
  lcd.print(day);
  lcd.setCursor(8,0);
  lcd.print("/");
  lcd.setCursor(9,0);
  lcd.print(month);
  lcd.setCursor(12,0);
  lcd.print("Temp");
  /*LCD hàng 2 */
    lcd.setCursor(0,1);
  lcd.print(gio_no);
  lcd.setCursor(2,1);
  lcd.print(":");
  lcd.setCursor(3,1);
  lcd.print(phut_no);
  lcd.setCursor(6,1);
  lcd.print(ngay_no);
  lcd.setCursor(8,1);
  lcd.print("/");
  lcd.setCursor(9,1);
  lcd.print(thang_no);
  lcd.setCursor(12,1);
  lcd.print(temp);
  lcd.setCursor(14,1);
  lcd.print("*C");
  //
  dieuNhiet();
  if ( hour == gio_no && minute == phut_no && day == ngay_no && month == thang_no) {
  thongBao();
  }
}
void dieuNhiet() {
  if (temp < (nhiet_do_no - 1)) {
    digitalWrite(chanDen, HIGH);  
    digitalWrite(chanQuat, LOW); 
  } else if (temp > (nhiet_do_no + 1)) {
    digitalWrite(chanDen, LOW);  
    digitalWrite(chanQuat, HIGH);  
  } else {
    digitalWrite(chanDen, LOW);  
    digitalWrite(chanQuat, LOW); 
  }
}
void thongBao() {
  coiBaoHieu = true;
  time = millis();
  
  while (coiBaoHieu) {
    digitalWrite(chanCoi, HIGH); // Kích hoạt chuông báo
    delay(10000); // Kêu chuông trong 10 giây
    
    digitalWrite(chanCoi, LOW); // Tắt chuông báo
    delay(300000); // Đợi 5 phút
    
    // Kiểm tra nút "button_OK" để tắt chuông báo
    if (digitalRead(button_OK) == 0) {
      coiBaoHieu = false;
    }
  }
}
/* cài đặt thời gian cho DS3231 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr) {
  Wire.beginTransmission(DS3231);
  Wire.write(byte(0x00));  // đặt lại pointer
  Wire.write(dec2bcd(sec));
  Wire.write(dec2bcd(min));
  Wire.write(dec2bcd(hr));
  Wire.write(dec2bcd(wd));  // day of week: Sunday = 1, Saturday = 7
  Wire.write(dec2bcd(d));
  Wire.write(dec2bcd(mth));
  Wire.write(dec2bcd(yr));
  Wire.endTransmission();
}
void Mode1(){
  if(digitalRead(button_OK)==0)
  {
      if(statusmode1 = 4) statusmode1=0;
    else statusmode1++;
  }
  if (statusmode1==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set up time:");
  }
    if (statusmode1==1){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set up time:HOUR");
    if(digitalRead(button_up)==0)
  {
      if(gio_no = 23) gio_no=0;
    else gio_no++;
  }
  if(digitalRead(button_down)==0)
  {
      if(gio_no = 0) gio_no=23;
    else gio_no--;
  }
    }
    if (statusmode1==2){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set up time: MIN");
    if(digitalRead(button_up)==0)
  {
      if(phut_no = 59) phut_no=0;
    else phut_no++;
  }
  if(digitalRead(button_down)==0)
  {
      if(phut_no = 0) phut_no=59;
    else phut_no--;
  }
    }
    if (statusmode1==3){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set up time: DAY");
    if(digitalRead(button_up)==0)
  {
      if(ngay_no = 31) ngay_no=1;
    else ngay_no++;
  }
  if(digitalRead(button_down)==0)
  {
      if(ngay_no = 1) ngay_no=31;
    else ngay_no--;
  }
  }
      if (statusmode1==4){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set up time: MON");
    if(digitalRead(button_up)==0)
  {
      if(thang_no = 12) thang_no=1;
    else thang_no++;
  }
  if(digitalRead(button_down)==0)
  {
      if(thang_no = 1) thang_no=12;
    else thang_no--;
  }
    }
    lcd.setCursor(0,1);
    lcd.print(gio_no);
    lcd.setCursor(2,1);
    lcd.print(":");
    lcd.setCursor(3,1);
    lcd.print(phut_no);
    lcd.setCursor(6,1);
    lcd.print(ngay_no);
    lcd.setCursor(8,1);
    lcd.print("/");
    lcd.setCursor(9,1);
    lcd.print(thang_no);
    
}
void Mode2(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Set up temp");
    if(digitalRead(button_up)==0)
  {
      if(nhiet_do_no = 50) nhiet_do_no=30;
    else nhiet_do_no++;
  }
  if(digitalRead(button_down)==0)
  {
      if(nhiet_do_no = 30) nhiet_do_no=50;
    else nhiet_do_no--;
  }
     lcd.setCursor(0,1);
    lcd.print(nhiet_do_no);
    lcd.setCursor(2,1);
    lcd.print(" do C");
    }
