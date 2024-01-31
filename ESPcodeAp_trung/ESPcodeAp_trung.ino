#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
 
char auth[] = "TANQNFpxFNal5GGGBt846IRf1XLvB2ZB";
 
char ssid[] = "Trinh Quang Nam.";
char pass[] = "bubu123789";
 
SimpleTimer timer;
 
String myString; // complete message from arduino, which consistors of snesors data
char rdata; // received charactors
 
int firstVal, secondVal,thirdVal; // sensors 
// void myTimerEvent()
// {

//   Blynk.virtualWrite(V1, millis() / 1000);
  
// }
 
 
 
void setup()
{
  // Debug console
  Serial.begin(9600);
 
  Blynk.begin(auth, ssid, pass);
 
    timer.setInterval(1000L,sensorvalue1); 
    timer.setInterval(1000L,sensorvalue2); 
 
}
 
void loop()
{
   if (Serial.available() == 0 ) 
   {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
   }
   
  if (Serial.available() > 0 ) 
  {
    rdata = Serial.read(); 
    myString = myString+ rdata; 
   // Serial.print(rdata);
    if( rdata == '\n')
    {

String l = getValue(myString, ',', 0);
String m = getValue(myString, ',', 1);
String n = getValue(myString, ',', 2); 
 
 
firstVal = l.toInt();
secondVal = m.toInt();
thirdVal = n.toInt();
 
  myString = "";
// end new code
    }
  }
 
}
 
void sensorvalue1()
{
int sdata = firstVal;

  Blynk.virtualWrite(V2, sdata);
 
}
void sensorvalue2()
{
int sdata = secondVal;

  Blynk.virtualWrite(V3, sdata);
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