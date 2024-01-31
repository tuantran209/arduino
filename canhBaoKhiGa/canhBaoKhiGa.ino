#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6U4hyPNu0"
#define BLYNK_TEMPLATE_NAME "ESMARTMQ5"
#define BLYNK_AUTH_TOKEN "xJolB7KwLddQDeMi5HPqNKjoCoDr_ZL9" // Đây là mã xác thực (authentication token) của ứng dụng Blynk
                      // Nó được sử dụng để xác định thiết bị và ứng dụng trong quá trình kết nối.

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "603";
char pass[] = "603603603";

BlynkTimer timer;
/*Khai báo các chân và biến */
const int digMQ5_PIN = D1;
const int SFM27_PIN = D5;
const int anlMQ5_PIN = A0;
int gasValue = 0;
int warningValue = 1023;
int runMode = 1;
int canhbaoState = 0;

void setup(){

 Serial.begin(115200);
 Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
 pinMode(D1, INPUT);
 pinMode(A0, INPUT);
 pinMode(D5, OUTPUT);
 timer.setInterval(1000L, sendUptime);  
 // Hàm thiết lập sự kiện lặp lại theo chu kỳ thời gian được chỉ định
}

void sendUptime(){
 gasValue = analogRead(anlMQ5_PIN);
 Blynk.virtualWrite(V1, gasValue);
 Blynk.virtualWrite(V0, HIGH);
 if(runMode == 1){
  if (gasValue >= warningValue) {
   if(canhbaoState == 0){
    canhbaoState = 1;
    Blynk.logEvent("canh_bao", String("Cảnh báo! Khí gas = " + String(gasValue)+" vượt quá mức cho phép!"));// gui canh bao ve email
   }
 Blynk.virtualWrite(V3, HIGH);
 digitalWrite(D5, HIGH);
 Serial.print("Canh bao! Gia tri khi Gas vuot qua cho phep: ");
 Serial.println(gasValue);
 }
  else{
 canhbaoState = 0;
 Blynk.virtualWrite(V3, LOW);
 Serial.print("Gas value is: ");
 Serial.println(gasValue);
 }
 }
 else{
  canhbaoState = 0;
  Blynk.virtualWrite(V3, LOW);
  digitalWrite(D5, LOW);
 }
}

BLYNK_CONNECTED() {
 Blynk.syncVirtual(V2,V4);
}

BLYNK_WRITE(V2)
{
warningValue = param.asInt(); // assigning incoming value from pin V2 to a variable// thay doi gia tri tren Blynk
 Serial.print("Warning value is: ");
 Serial.println(warningValue);
}

BLYNK_WRITE(V4)
{
runMode = param.asInt(); // assigning incoming value from pin V4 to a variable
 if(runMode == 1){
 Serial.println("Trang thai hoat dong: Dang hoat dong");
 }
 else{
   Serial.println("Trang thai hoat dong: Tat");
 }
}

void loop()
{
 Blynk.run();
 timer.run();
}
