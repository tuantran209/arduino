#include <SPI.h> 
#include <nRF24L01.h>
#include <RF24.h>
#define CE_PIN   9
#define CSN_PIN 10
#define x_axis A0 // x axis
#define y_axis A1 //y axis
#define button1 8 // joystick button
#define button2 2 // A button
#define button3 3 // B button
#define button4 4 // C button
#define button5 5 // D button
#define button6 6 // E button
#define button7 7 // F button
const uint64_t pipe = 0xE8E8F0F0E1LL; 
RF24 radio(CE_PIN, CSN_PIN); 
int data[9];  

void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(button5, INPUT);
  pinMode(button6, INPUT);
  pinMode(button7, INPUT);
}

void loop()   
{

  data[0] = analogRead(x_axis);
  data[1] = analogRead(y_axis);
  data[2] = digitalRead(button1);
  data[3] = digitalRead(button2);
  data[4] = digitalRead(button3);
  data[5] = digitalRead(button4);
  data[6] = digitalRead(button5);
  data[7] = digitalRead(button6);
  data[8] = digitalRead(button7);
  radio.write( data, sizeof(data) ); 
//HATA AYIKLAMA (DEBUG)
  Serial.print("X = ");
  Serial.println(analogRead(x_axis));
  Serial.print("Y = ");
  Serial.println(analogRead(A1));
}