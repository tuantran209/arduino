///>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Test Database Access 
//======================================== Including the libraries.
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include <DNSServer.h>
#include <WiFiManager.h> 
WiFiManager wifiManager;
#include <Arduino_JSON.h>
//======================================== 

#define ON_Board_LED D3 
#define BULD D4 
#define PUMP D5 

//======================================== SSID and Password of your WiFi router.
const char* ssid = "TP-Link_9F28";
const char* password = "79606486";
//======================================== 

//======================================== Variables for HTTP POST request data.
String postData = ""; //--> Variables sent for HTTP POST request data.
String payload = "";  //--> Variable for receiving response from HTTP POST.
//======================================== 

//======================================== Variables for DHT11 sensor data.
float send_Temp = 1;
int send_Humd = 2;
String send_Status_Read_DHT11 = "";
//======================================== 
//________________________________________________________________________________ Subroutine to control LEDs after successfully fetching data from database.
void control_LEDs() {
  Serial.println();
  Serial.println("---------------control_LEDs()");
  JSONVar myObject = JSON.parse(payload);

  // JSON.typeof(jsonVar) can be used to get the type of the var
  if (JSON.typeof(myObject) == "undefined") {
    Serial.println("Parsing input failed!");
    Serial.println("---------------");
    return;
  }

  if (myObject.hasOwnProperty("BULD")) {
    Serial.print("myObject[\"BULD\"] = ");
    Serial.println(myObject["BULD"]);
  }

  if (myObject.hasOwnProperty("PUMP")) {
    Serial.print("myObject[\"PUMP\"] = ");
    Serial.println(myObject["PUMP"]);
  }

  if(strcmp(myObject["BULD"], "ON") == 0)   {digitalWrite(BULD, HIGH);  Serial.println("LED 01 ON"); }
  if(strcmp(myObject["BULD"], "OFF") == 0)  {digitalWrite(BULD, LOW);   Serial.println("LED 01 OFF");}
  if(strcmp(myObject["PUMP"], "ON") == 0)   {digitalWrite(PUMP, HIGH);  Serial.println("LED 02 ON"); }
  if(strcmp(myObject["PUMP"], "OFF") == 0)  {digitalWrite(PUMP, LOW);   Serial.println("LED 02 OFF");}

  Serial.println("---------------");
}
//________________________________________________________________________________ 

// ________________________________________________________________________________ Subroutine to read and get data from the DHT11 sensor.
void get_DHT11_sensor_data() {
  Serial.println();
  Serial.println("-------------get_DHT11_sensor_data()");
  if (isnan(send_Temp) || isnan(send_Humd)) {
    Serial.println("Failed to read from DHT sensor!");
    send_Temp = 0.00;
    send_Humd = 0;
    send_Status_Read_DHT11 = "FAILED";
  } else {
    send_Status_Read_DHT11 = "SUCCEED";
  }
  
  Serial.printf("Temperature : %.2f °C\n", send_Temp);
  Serial.printf("Humidity : %d %%\n", send_Humd);
  Serial.printf("Status Read DHT11 Sensor : %s\n", send_Status_Read_DHT11);
  Serial.println("-------------");
}
// ________________________________________________________________________________

//________________________________________________________________________________ VOID SETUP()
void setup() {
  Serial.begin(115200); //--> Initialize serial communications with the PC.

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output.
  pinMode(BULD,OUTPUT); //--> BULD port Direction output.
  pinMode(PUMP,OUTPUT); //--> PUMP port Direction output.
  
  digitalWrite(ON_Board_LED, HIGH); //--> Turn on Led On Board.
  digitalWrite(BULD, HIGH); //--> Turn on BULD.
  digitalWrite(PUMP, HIGH); //--> Turn on PUMP.

  delay(2000);

  digitalWrite(ON_Board_LED, LOW); //--> Turn off Led On Board.
  digitalWrite(BULD, LOW); //--> Turn off Led BULD.
  digitalWrite(PUMP, LOW); //--> Turn off Led PUMP.

  //---------------------------------------- Make WiFi on ESP32 in "STA/Station" mode and start connecting to WiFi Router/Hotspot.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //---------------------------------------- 
  
  Serial.println();
  Serial.println("-------------");
  Serial.print("Connecting");

  int connecting_process_timed_out = 20; //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");

    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    if(connecting_process_timed_out > 0) connecting_process_timed_out--;
    if(connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
    //........................................ 
  }
  //---------------------------------------- 
  
  digitalWrite(ON_Board_LED, LOW); //--> Turn off the On Board LED when it is connected to the wifi router.
  
  Serial.println();
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
  Serial.println("-------------");

  delay(2000);
}
//________________________________________________________________________________ 

//________________________________________________________________________________ VOID LOOP()
void loop() {
  WiFiClient client;
  //---------------------------------------- Check WiFi connection status.
  if(WiFi.status()== WL_CONNECTED) {
    HTTPClient http;  //--> Declare object of class HTTPClient.
    int httpCode;     //--> Variables for HTTP return code.
    
    //........................................ Process to get LEDs data from database to control LEDs.
    postData = "id=esp32_01";
    payload = "";
  
    digitalWrite(ON_Board_LED, HIGH);
    Serial.println();
    Serial.println("---------------getdata.php");
    // In this project I use local server or localhost with XAMPP application.
    // So make sure all PHP files are "placed" or "saved" or "run" in the "htdocs" folder.
    // I suggest that you create a new folder for this project in the "htdocs" folder.
    // The "htdocs" folder is in the "xampp" installation folder.
    // The order of the folders I recommend:
    // xampp\htdocs\your_project_folder_name\phpfile.php
    //
    // ESP32 accesses the data bases at this line of code: 
    // http.begin("http://REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS/REPLACE_WITH_PROJECT_FOLDER_NAME_IN_htdocs_FOLDER/getdata.php");
    // REPLACE_WITH_YOUR_COMPUTER_IP_ADDRESS = there are many ways to see the IP address, you can google it. 
    //                                         But make sure that the IP address used is "IPv4 address".
    // Example : http.begin("http://192.168.0.0/ESP32_MySQL_Database/Final/getdata.php");
    http.begin(client,"http://192.168.0.101/doan/getdata.php");;  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");        //--> Specify content-type header
   
    httpCode = http.POST(postData); //--> Send the request
    payload = http.getString();     //--> Get the response payload
  
    Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload
    
    http.end();  //--> Close connection
    Serial.println("---------------");
    digitalWrite(ON_Board_LED, LOW);
    //........................................ 

    // Calls the control_LEDs() subroutine.
    control_LEDs();
    
    delay(1000);
    get_DHT11_sensor_data();
    String BULD_State = "";
    String PUMP_State = "";

    if (digitalRead(BULD) == 1) BULD_State = "ON";
    if (digitalRead(BULD) == 0) BULD_State = "OFF";
    if (digitalRead(PUMP) == 1) PUMP_State = "ON";
    if (digitalRead(PUMP) == 0) PUMP_State = "OFF";
    
    postData = "id=esp32_01";
    postData += "&temperature=" + String(send_Temp);
    postData += "&humidity=" + String(send_Humd);
    postData += "&soil_humidity=" + String(send_Temp);
    postData += "&lux=" + String(send_Humd);
    postData += "&status_read_sensor_dht11=" + send_Status_Read_DHT11;
    postData += "&status_read_sensor_soil=" + send_Status_Read_DHT11;
    postData += "&status_read_sensor_light=" + send_Status_Read_DHT11;
    postData += "&BULD=" + BULD_State;
    postData += "&PUMP=" + PUMP_State;
    payload = "";
  
    digitalWrite(ON_Board_LED, HIGH);
    Serial.println();
    Serial.println("---------------updatesDataAndRecord.php");
    // Example : http.begin("http://192.168.0.0/ESP32_MySQL_Database/Final/updateDHT11data_and_recordtable.php");
     http.begin(client,"http://192.168.0.101/doan/updatesDataAndRecord.php");  //--> Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  //--> Specify content-type header
   
    httpCode = http.POST(postData); //--> Send the request
    payload = http.getString();  //--> Get the response payload
  
    Serial.print("httpCode : ");
    Serial.println(httpCode); //--> Print HTTP return code
    Serial.print("payload  : ");
    Serial.println(payload);  //--> Print request response payload
    
    http.end();  //Close connection
    Serial.println("---------------");
    digitalWrite(ON_Board_LED, LOW);
    //........................................ 
    
    delay(4000);
  }
  //---------------------------------------- 
}


