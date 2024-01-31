#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include<Servo.h>
#define CE_PIN   9
#define CSN_PIN 10

Servo ESC1; 
Servo ESC2;  
Servo ESC3; 
Servo ESC4; 

int x_hiz=0;
int y_hiz=0;



const uint64_t pipe = 0xE8E8F0F0E1LL;

RF24 radio(CE_PIN, CSN_PIN); 

int data[9]; 

void setup()   
{

  Serial.begin(9600);

  ESC2.attach(8);
  ESC3.attach(7);
  ESC4.attach(6);
  ESC1.attach(5);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();;
}


void loop() {
  if ( radio.available() ) //Eğer sinyal algılarsan...
  {
    radio.read( data, sizeof(data) ); 
    

Serial.print("data[0]=");
Serial.println(data[0]);
Serial.print("data[1]=");
Serial.println(data[1]);

hareket(data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8]);

  }

}

void hareket(int x_axis, int y_axis, int button1, int button2, int button3, int button4, int button5, int button6, int button7) {
 
 x_hiz= map(x_axis,600,1023,1000,2000);
 y_hiz= map(y_axis,600,1023,1000,2000);
  ESC1.writeMicroseconds(x_hiz);
  ESC2.writeMicroseconds(x_hiz);
  ESC3.writeMicroseconds(x_hiz);
  ESC4.writeMicroseconds(x_hiz);
  }
