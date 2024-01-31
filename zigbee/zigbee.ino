String string = "node3";
void setup() {
  
  Serial.begin(38400);  //Mở cổng Serial để giap tiếp | tham khảo Serial
}
 
void loop() {
  int value = analogRead(A0);   //đọc giá trị điện áp ở chân A0 - chân cảm biến

  Serial.println(string+" "+value);        //xuất ra giá trị vừa đọc
  
  
  //-----------------------------------------------------
  delay(1000);           //đợi 1 giây để bạn kịp tháy serial - (optional)
}
