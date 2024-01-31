
#include <SoftwareSerial.h>
#include <BH1750FVI.h>
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
SoftwareSerial mySerial(6, 7); //TX, RX
#define M0 8
#define M1 9                 
#include <ArduinoJson.h>
int lux;
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  LightSensor.begin();
  pinMode(M0, OUTPUT);         
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);       // Set 2 chân M0 và M1 xuống LOW 
  digitalWrite(M1, LOW);       // để hoạt động ở chế độ+ Normal
  delay(1000);
}
void light(){
  lux = 10; 
  //Serial.print("Light: ");
  Serial.println(lux);
}
void loop() {
StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  //Obtain Temp and Hum data
  //Assign collected data to JSON Object
  light();
  data["address"] = 2;
  data["lux"] = lux;
  data.printTo(mySerial);
  jsonBuffer.clear();

  delay(1500);
}
  
