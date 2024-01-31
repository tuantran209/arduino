#include <SoftwareSerial.h>
#include "DHT.h"

SoftwareSerial nodemcu(1, 0); //tx-1, rx-0

#define DHTPIN 9
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int temp;
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
//int ngay_no = 31, thang_no = 12, gio_no = 23, phut_no = 59, nhiet_do_no = 30;
void setup()
{
  Serial.begin(9600);
  nodemcu.begin(9600);
  dht.begin();
  pinMode(4, OUTPUT); // bat led
  digitalWrite(4, LOW);
}

void loop()
{
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
  
  if (sdata1 > 500) {
    digitalWrite(chanLed, HIGH);
  } else {
    digitalWrite(chanLed, LOW);
  }
  cdata = String(sdata1) + "," + String(sdata2) + "," + String(sdata3) + "," + String(sdata4) + "," + String(sdata5)+","+String(sdata6) + "," + String(sdata7) + "," + String(sdata8) + "," + String(sdata9);
  Serial.println(cdata);
  nodemcu.println(cdata);
  
  delay(2000); // 2 seconds
  cdata = "";
}
