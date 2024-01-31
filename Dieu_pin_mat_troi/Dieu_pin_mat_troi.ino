#include <Servo.h>
#define pin_servo_h 9 // servo dieu khien truc ngang
#define pin_servo_v 8 // servo dieu khien truc doc
#define pin_left_top A0 // quang tro o goc trai tren
#define pin_right_top A1 // quang tro o goc phai tren
#define pin_left_bot A2 // quang tro o goc trai duoi
#define pin_right_bot A3 // quang tro o goc phai duoi
#define tol 15

Servo servo_h;
int servo_h_angle = 90;
Servo servo_v;
int servo_v_angle = 90;
void setup() 
{   
  Serial.begin(9600);
  servo_h.attach(pin_servo_h);
  servo_v.attach(pin_servo_v);
  servo_h.write(servo_h_angle);
  servo_v.write(servo_v_angle);  
  pinMode(pin_left_top, INPUT);
  pinMode(pin_right_top, INPUT);
  pinMode(pin_left_bot, INPUT);
  pinMode(pin_right_bot, INPUT);
}
 
void loop() 
{
  /*nhan gia tri quang tro o cac vi tri */
  int left_top = analogRead(pin_left_top);
  int right_top = analogRead(pin_right_top);
  int left_bot = analogRead(pin_left_bot);
  int right_bot = analogRead(pin_right_bot);
 
  /*tinh gia tri trung binh */
  int avt = (left_top + right_top) / 2;    //phia tren
  int avb = (left_bot + right_bot) / 2;    // phia duoi
  int avl = (left_top + left_bot) / 2 ;    // ben trai
  int avr = (right_top + right_bot) / 2 ;  //ben phai

  /*tinh do lech*/
  int dv  = avt - avb;
  int dh  = avl-avr;
 /* dieu huong truc ngang */
 Serial.println(dv);
 Serial.println(dh);
 Serial.println("--------------------------------");
 if ( dv > tol || -1*tol > dv)
  {
    if (avt < avb)
    {
      servo_v_angle +=3;
      if (servo_v_angle > 180) servo_v_angle = 180;
    }
    else
    {
      servo_v_angle -=3;
      if (servo_v_angle < 0) servo_v_angle = 0;
    }
    servo_v.write(servo_v_angle);
  }
 if ( dh > tol || -1*tol > dh)
  {
    if (avl > avr)
    {
      servo_h_angle +=3;
      if (servo_h_angle > 180) servo_h_angle = 180;
    }
    else
    {
      servo_h_angle -=3;
      if (servo_h_angle < 0) servo_h_angle = 0;
    }
    servo_h.write(servo_h_angle);
  }
  delay(50);
}
