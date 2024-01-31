#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial nodemcu(D8, D7);// TX-D8 RX-D7 ;

#define BLYNK_TEMPLATE_ID "TMPL60yYFA5gF"
#define BLYNK_TEMPLATE_NAME "TuanTran209"
#define BLYNK_AUTH_TOKEN "kWh77eECSapAC4GPcF0mybSw1WEWE9LP"

 /*Tên , Mật khẩu Wifi*/
char ssid[] = "P504 5G VNPT";
char pass[] = "12345689";
  String myString; // complete message from arduino, which consistors of snesors data
  char rdata; // received charactors
  int gio_no;
  int phut_no;
  int ngay_no;
  int thang_no;
  int nhiet_do_no;
  int hour;
  int minute;
  int day;
  int month;
  int temp;

void setup()
{
  // Debug console
   Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  nodemcu.begin(9600);
}

void loop()
{
   if (nodemcu.available() == 0 ) 
   {
  Blynk.run(); 
   }
   
  if (nodemcu.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
    Serial.print(rdata);
    if( rdata == '\n')
    {

      String a = getValue(myString, ',', 0);
      String b = getValue(myString, ',', 1);
      String c = getValue(myString, ',', 2); 
      String d = getValue(myString, ',', 3);
      String e = getValue(myString, ',', 4);
      String f = getValue(myString, ',', 5); 
      String g = getValue(myString, ',', 6);
      String h = getValue(myString, ',', 7);
      String i = getValue(myString, ',', 8); 
      String k = getValue(myString, ',', 9);

      //đổi giá trị thành số nguyên
      gio_no      = a.toInt();
      phut_no     = b.toInt();
      ngay_no     = c.toInt();
      thang_no    = d.toInt();
      nhiet_do_no = e.toInt();
      hour        = f.toInt();
      minute      = g.toInt();
      day         = h.toInt();
      month       = i.toInt();
      temp        = k.toInt();
        myString = "";
      // end new code
    }
  }
  Blynk.virtualWrite(V1,gio_no);
  Blynk.virtualWrite(V2,phut_no);
  Blynk.virtualWrite(V3,ngay_no);
  Blynk.virtualWrite(V4,thang_no);
  Blynk.virtualWrite(V5,nhiet_do_no);
  Blynk.virtualWrite(V6,hour);
  Blynk.virtualWrite(V7,minute);
  Blynk.virtualWrite(V8,day);
  Blynk.virtualWrite(V9,month);
  Blynk.virtualWrite(V10,temp);

}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;
 
    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

