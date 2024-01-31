#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <U8g2lib.h>

/* blynk  */
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL601Q5y3b1"
#define BLYNK_TEMPLATE_NAME "Hệ thống chăm sóc cây trồng tự động"
#define BLYNK_AUTH_TOKEN "GZxxzZrlZq0MHJb6Ilgjw3jU-Ua7YIOW"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char ssid[] = "Embo123";
char pass[] = "trungpham";
/*         */
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,D1,D2,U8X8_PIN_NONE); // khởi tạo oled
SoftwareSerial mySerial(D7, D8); //TX, RX
#define M0 D5
#define M1 D6                
void setup() {
  u8g2.begin();
  Serial.begin(115200);
  mySerial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(D2,OUTPUT);
  pinMode(M0, OUTPUT);        
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);       // Set 2 chân M0 và M1 xuống LOW 
  digitalWrite(M1, LOW); 
    while (!Serial) continue;      // để hoạt động ở chế độ Normal
}
float hum,temp;
int soil_humi;

void shownode1(){
  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, soil_humi);
  char tempstr[2];
  char humstr[2];
  char Soil_humstr[2];
  u8g2.clearBuffer();
  sprintf(tempstr, "%.1f", temp);   //chuyển nhiệt độ sang kiểu xâu kí tự
  u8g2.setFont(u8g2_font_ncenB08_tr); // Chọn font chữ
  u8g2.drawStr(2,20, "NODE 1");
  u8g2.drawStr(2,30, "Nhiet do: "); // Hiển thị lên oled
  u8g2.drawStr(55,30,tempstr);  
  u8g2.drawStr(78,30,"*C");  
  sprintf(humstr, "%.1f", hum); // chuyển độ ẩm sang kiểu xâu kí tự
  u8g2.drawStr(2,40, "Do am khong khi: ");    
  u8g2.drawStr(97,40, humstr);  
  u8g2.drawStr(118,40, "%");
  sprintf(Soil_humstr, "%d", soil_humi); // chuyển độ ẩm sang kiểu xâu kí tự
  u8g2.drawStr(2,51, "Do am dat: ");    
  u8g2.drawStr(61,51, Soil_humstr);  
  u8g2.drawStr(73,51, "%");    
  u8g2.sendBuffer();           // Hiển thị nội dung từ bộ đệm lên màn hình  
}
void loop() {
/*nhận dữ liệu từ Lora qua UART */
StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(mySerial);
  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }
  /*nhận data node 1*/
  if (data["address"]==1){
  Serial.println("NODE 1: ");
  Serial.print("Recieved Humidity:  ");
  hum = data["humidity"];//
  Serial.println(hum);
  Serial.print("Recieved Temperature:  ");
  temp = data["temperature"];
  Serial.println(temp);
  Serial.print("Recieved Soil_humidity:  ");
  Serial.println(soil_humi);
  Serial.println("-----------------------------------------");
  soil_humi = data["soil_humidity"];

  }

  /*Nhận data node 2 */
  if (data["address"]==2){
  Serial.println("NODE 2: ");
  Serial.print("Recieved light:  ");
  int lux = data["lux"];
  Serial.println(lux);
  Serial.println("-----------------------------------------");
  }
  shownode1();
  Blynk.run();
  delay(1000);

}
