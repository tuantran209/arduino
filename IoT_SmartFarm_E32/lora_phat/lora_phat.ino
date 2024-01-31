#include <SoftwareSerial.h>
#include <DHT.h>
SoftwareSerial mySerial(6, 7);  //TX, RX
#define M0 8
#define M1 9
#include <ArduinoJson.h>
#define DHTPIN 5
#define maybom 2
#define den_maybom 3
#define quat 4
DHT dht(DHTPIN, DHT11);
float temp;
float hum;
int soil_humi;
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  dht.begin();
  /*set up cac chan che do cua lora */
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);  // Set 2 chân M0 và M1 xuống LOW
  digitalWrite(M1, LOW);  // để hoạt động ở chế độ Normal

  /* set up cac chan cua cam bien*/
  pinMode(5, OUTPUT);  // cam bien DHT11
  pinMode(A0, INPUT);  // Cam bien do am dat

  /* set up cac chan thiet bi chap hanh*/
  pinMode(2, OUTPUT);  // relay dieu khien may bom
  pinMode(3, OUTPUT);  // den bao hieu do am dat
  pinMode(4, OUTPUT);  // relay dieu khien quat
  delay(1000);
}
void dht11_func() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Temperature: ");
  Serial.println(temp);
  if (temp > 25.7) digitalWrite(quat, HIGH);
  else if (temp < 20) digitalWrite(quat, LOW);
}
void Do_do_am_dat() {
  int value0 = analogRead(A0);
  int value1 = analogRead(A1);
  soil_humi = (map(value0, 200, 1023, 100, 0) + map(value1, 200, 1023, 100, 0))/2;
  Serial.print("Soil humidity: ");
  Serial.println(soil_humi);
  if (soil_humi < 60) {
    digitalWrite(maybom, LOW);
    digitalWrite(den_maybom, HIGH);
  } else if (soil_humi > 80) {
    digitalWrite(maybom, HIGH);
    digitalWrite(den_maybom, LOW);
  }
}
void loop() {
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  //Obtain Temp and Hum data
  dht11_func();
  Do_do_am_dat();
  //Assign collected data to JSON Object
  data["address"] = 1;  // địa chỉ node
  data["humidity"] = hum;
  data["temperature"] = temp;
  data["soil_humidity"] = soil_humi ;
  //Send data to NodeMCU
  Serial.println("-------------------------------------------------------");
  data.printTo(mySerial);
  jsonBuffer.clear();
  delay(1000);
}
