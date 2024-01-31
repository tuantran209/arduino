#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, D1, D2, U8X8_PIN_NONE); // khởi tạo oled
SoftwareSerial mySerial(D7, D8); // TX, RX
#define M0 D5
#define M1 D6

void setup() {
  u8g2.begin();
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(D2, OUTPUT);
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW); // Set 2 chân M0 và M1 xuống LOW 
  digitalWrite(M1, LOW);
  while (!Serial); // Đợi kết nối với Serial Monitor để hoạt động ở chế độ Normal
}

void loop() {
  // Kiểm tra xem có dữ liệu để đọc từ Serial không
  if (mySerial.available()) {
    // Nhận dữ liệu từ Lora qua UART
    DynamicJsonDocument jsonDocument(1000); // Sử dụng DynamicJsonDocument thay vì StaticJsonBuffer
    DeserializationError error = deserializeJson(jsonDocument, mySerial);
  
    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }
  
    // Nhận data node 1
    int address = jsonDocument["address"];
    if (address == 1) {
      Serial.println("NODE 1: ");
      float hum = jsonDocument["humidity"];
      float temp = jsonDocument["temperature"];
      float val_lux = jsonDocument["light"];
      float val_soil = jsonDocument["soil"];
      Serial.print("Received Humidity:  ");
      Serial.println(hum);
      Serial.print("Received Temperature:  ");
      Serial.println(temp);
      Serial.print("Received Light:  ");
      Serial.println(val_lux);
      Serial.print("Received Soil:  ");
      Serial.println(val_soil);
      Serial.println("-----------------------------------------");
    }
  }
}
