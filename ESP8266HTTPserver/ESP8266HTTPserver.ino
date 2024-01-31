#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <WiFiManager.h> 
WiFiManager wifiManager;

const char *host = "192.168.0.101";   

#define bt D5
void setup() {
  long t = millis();
  pinMode(bt, INPUT); //bt wire D5 and 3.3V
  EEPROM.begin(512);
  Serial.begin(9600);
  delay(3000);
  if (digitalRead(bt) == 1) {
  wifiManager.resetSettings();
  delay(1000);
  }
  wifiManager.autoConnect("ESP8266");
  Serial.println("connected...ok :)");
  ////////////////////
  delay(1000);
  // Serial.begin(115200);
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
}
void loop() {
  WiFiClient client;
  HTTPClient http;   
  String postData;
  //Post Data
  postData = "mathietbi=" + String(random(10,9999))
   + "&kinhdo=" + String(random(00,99)) + "&vido=" + String(random(00,99)) + "&kc=" + String(random(00,99)) ;
  Serial.println(postData);
  http.begin(client,"http://192.168.0.101/demo/postdemo.php");             
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");   

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}