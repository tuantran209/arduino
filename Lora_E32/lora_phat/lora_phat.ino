#include <SoftwareSerial.h>
#include <DHT.h>
#include <ArduinoJson.h>
SoftwareSerial mySerial(6, 7); // TX, RX
#define M0 8
#define M1 9
#define DHTPIN 5
#define Lux A0
#define Soild A1
DHT dht(DHTPIN, DHT11);

float temp;
float hum;
float val_lux;
float val_soil;

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  dht.begin();
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(Lux, INPUT);
  pinMode(Soild, INPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);
  delay(1000);
}

void dht11_func() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();
}

void loop() {
  DynamicJsonDocument jsonDocument(1000); // Sử dụng DynamicJsonDocument thay vì StaticJsonBuffer
  JsonObject data = jsonDocument.to<JsonObject>();
  // Nhận dữ liệu nhiệt độ và độ ẩm ánh sáng độ ẩm đất

  int sensorValue1 = analogRead(Lux); 
  val_lux=(float)(1023-sensorValue1)*10000/sensorValue1;

  int sensorValue2 = analogRead(Soild); 
  val_soil =map(sensorValue2,0,1023,100,0);

  dht11_func();
  // Gán dữ liệu đã thu thập vào JSON Object
  data["address"] = 1; // Địa chỉ node
  data["humidity"] = hum;
  data["temperature"] = temp;
  data["light"] = val_lux;
  data["soil"] = val_soil;
  //kiem tra
  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Temperature: ");
  Serial.println(temp);
  Serial.print("Light: ");
  Serial.println(val_lux);
    Serial.print("Soil: ");
  Serial.println(val_soil);
  // Gửi dữ liệu đến NodeMCU
  serializeJson(data, mySerial);
  jsonDocument.clear(); // Giải phóng bộ nhớ sau khi sử dụng JSON document
  delay(1000);
  
  // if (temp > 25.7) {
  //   digitalWrite(4, HIGH);
  // } else {
  //   digitalWrite(4, LOW);
  // }
}
  
