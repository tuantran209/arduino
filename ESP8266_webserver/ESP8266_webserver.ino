#include <string.h>
#include <ESP8266WiFi.h>
//#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define bt D8
char auth[] = "Your token";
WiFiManager wifiManager;
String url = "http://192.168.5.129/serverDBS/pureba_recibe.php"; // Fixed the URL by removing double slashes

String device = "tarjeta2";
int temp = 0;
int hume = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
void setup()
{
  long t = millis();
  pinMode(bt, INPUT); //bt wire D8 and 3.3V
  EEPROM.begin(512);
  Serial.begin(9600);
  delay(3000);
  if (digitalRead(bt) == 1)
  {
    wifiManager.resetSettings();
    delay(1000);
  }

  wifiManager.autoConnect("ESP8266");
  Serial.println("connected...ok :)");
  //Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
}

void loop()
{
      WiFiClient client;
      HTTPClient http;
      http.begin(client, url);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      temp = random(1, 60);
      hume = random(10, 99);
      String postData = "device_label=" + device + "&temperature=" + String(temp) + "&humidity=" + String(hume);
      Serial.println(postData);
      int httpCode = http.POST(postData);
      String resquesta = http.getString();
       Serial.println(httpCode);
      Serial.println(resquesta);
      http.end();
      delay(500);
  }   