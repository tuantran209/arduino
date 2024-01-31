#include <SoftwareSerial.h>
void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);
  digitalWrite(M0, LOW);       // Set 2 chân M0 và M1 xuống LOW 
  digitalWrite(M1, LOW);       // để hoạt động ở chế độ Normal
}
void loop() {
  if(Serial.available() > 0){
    String input = Serial.readString();
    mySerial.println(input);    
  }
 
  if(mySerial.available() > 1){
    String input = mySerial.readString();
    Serial.println(input);    
  }
  delay(20);
}
