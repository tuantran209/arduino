
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

/* Set these to your desired credentials. */
const char *ssid = "GBS";  //ENTER YOUR WIFI SETTINGS
const char *password = "0101915236";

//Web/Server address to read/write from 
const char *host = "192.168.0.204";   


void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}
void loop() {
  WiFiClient client;
  HTTPClient http;    //Declare object of class HTTPClient
  String postData;
  //--int mathietbi, kinhdo,vido,khoangcach;
  //Post Data
  postData = "mathietbi=" + String(random(10,9999))
   + "&kinhdo=" + String(random(00,99)) + "&vido=" + String(random(00,99)) + "&kc=" + String(random(00,99)) ;
  Serial.println(postData);
  http.begin(client,"http://192.168.0.204/demo/postdemo.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header

  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
  
  delay(5000);  //Post Data at every 5 seconds
}