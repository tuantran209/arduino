#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //16x2 LCD on the 0x27 i2C address

float buttonTimer = 0; //Timer for avoiding multiple keypresses at a time
bool buttonPressed = false; //status which keeps track if a key was pressed
String fullString = ""; //concatenated string
String pressedCharacter = ""; //recently pressed character on the keypad
bool stringAdded = false; //keeps track of the concatenation
double convertedNumber = 0; //final number. fullString converted into double

/* !NOTICE!
 * There is no wiring schematics provided since I only used the i2C pins in this demonstration
 * SDA: A4, SCL: A5
 * Both the LCD and the PCF8574 module are compatible with 5 V
 * The pins on the keypad are connected to the PCF8574 in an ascending order:
 * P0: 1st pin on the keypad, P1: 2nd pin on the keypad...etc., P7: 8th pin on the keypad
 * Notice that the keypad's first actual pin is the 2nd physical pin (at least on the one I used)
 */

void setup()
{
  Wire.begin();
  Wire.setClock(400000L);
  Serial.begin(9600);
  Serial.println("PCF8574 with a keypad");
  //-----------------
  //LCD
  lcd.begin();
  lcd.setCursor(0, 0); //1st line, 1st block
  lcd.print("Keypad 123-ABC");
  lcd.setCursor(0, 1); //2nd line, 1st block
  lcd.print("Demonstration");
  //
  delay(2000); //wait 2 sec
  // Permanent parts
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("String: ");
  lcd.setCursor(0, 1);
  lcd.print("Double: ");
  //----------------------------------------------------------------------------
}

void loop()
{
  readKeyPad(); //read the keypad
  checkInput(); //do something if a key was pressed
}

void checkInput()
{
  if (buttonPressed == true) //if a button was pressed...
  {
    if (stringAdded == true) //if a numberical button was pressed...
    {
      fullString = fullString + pressedCharacter; //concatenate
      //Print the string on the LCD
      lcd.setCursor(8, 0);
      lcd.print(fullString);
      lcd.setCursor(8, 1); //remove the values from the 2nd line
      lcd.print("       ");
      stringAdded = false; //don't let the code enter this part again
    }

    if (pressedCharacter == "A") //if we pressed the A on the keypad...
    {
      //This part does the string -> double conversion, then resets everything
      Serial.print("String: ");
      Serial.println(fullString);
      convertedNumber = fullString.toDouble(); //string to double (because of the decimals)
      //Print the converted number on the LCD
      lcd.setCursor(8, 1);
      lcd.print(convertedNumber);
      Serial.println(convertedNumber);
      fullString = ""; //reset the string
      convertedNumber = 0; //reset the number to 0
      pressedCharacter = ""; //reset pressed character (this part will be skipped in further iterations until A is pressed again)
      lcd.setCursor(8, 0); //Clear the first line
      lcd.print("        ");
    }
    else if (pressedCharacter == "D") //D = delete
    {
      fullString = fullString.substring(0, fullString.length() - 1); // remove the last character
      Serial.print("String: ");
      Serial.println(fullString);
      lcd.setCursor(8 + fullString.length(), 0); //removes the last character from the LCD (abcd12|3)
      lcd.print(" ");
      lcd.setCursor(8, 0); //reprint the whole, 1 character shorter number
      lcd.print(fullString);
      pressedCharacter = "";
    }
  }
}


void readKeyPad()
{
  if (buttonPressed == true)
  {
    if (millis() - buttonTimer > 300)
    {
      //The button pressed is only set back to false after 300 ms, so we cannot press a button twice quickly
      buttonTimer = millis();
      buttonPressed = false;
    }
    else
    {
      //do nothing
    }
  }
  else
  {
    //B11101111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B11101111);
    //[P7]B11101111[P0] -> [P7]1110[P4] - activates first row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      //11101110 - P0 pin went low after pressing the button -> 1 was pressed
      case 238: //Button 1
        Serial.print("1");
        pressedCharacter = "1";
        buttonPressed = true;
        stringAdded = true;
        break;

      //11101101 - P1 pin went low after pressing the button -> 2 was pressed
      case 237: //Button 2
        Serial.print("2");
        pressedCharacter = "2";
        buttonPressed = true;
        stringAdded = true;
        break;

      //11101011 - P2 pin went low after pressing the button -> 3 was pressed
      case 235: //Button 3
        Serial.print("3");
        pressedCharacter = "3";
        buttonPressed = true;
        stringAdded = true;
        break;

      //11100111 - P3 pin went low after pressing the button -> A was pressed
      case 231: //Button A
        Serial.println("A");
        pressedCharacter = "A";
        buttonPressed = true;
        break;
    }
    //-------------------------------------------
    //B11011111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B11011111);
    //[P7]B11011111[P0] -> [P7]1101[P4] - activates second row, [P3]1111[P0] - Sets all pins high on the MUX
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      //11011110 - P0 pin went low after pressing the button -> 2 was pressed
      case 222: //Button 4
        Serial.print("4");
        pressedCharacter = "4";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 221: //Button 5
        Serial.print("5");
        pressedCharacter = "5";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 219: //Button 6
        Serial.print("6");
        pressedCharacter = "6";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 215: //Button B
        Serial.println("B");
        pressedCharacter = "B";
        buttonPressed = true;
        break;
    }
    //-------------------------------------------
    //B10111111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B10111111);
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      case 190: //Button 7
        Serial.print("7");
        pressedCharacter = "7";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 189: //Button 8
        Serial.print("8");
        pressedCharacter = "8";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 187: //Button 9
        Serial.print("9");
        pressedCharacter = "9";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 183: //Button C
        Serial.println("C");
        pressedCharacter = "C";
        buttonPressed = true;
        break;
    }
    //-------------------------------------------
    //B01111111
    Wire.beginTransmission(0x20); //00100000
    Wire.write(B01111111);
    Wire.endTransmission();

    Wire.requestFrom(0x20, 1);
    switch (Wire.read())
    {
      case 126: //Button *
        Serial.print(".");
        pressedCharacter = ".";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 125: //Button 0
        Serial.print("0");
        pressedCharacter = "0";
        buttonPressed = true;
        stringAdded = true;
        break;

      case 123: //Button #
        Serial.print("#");
        pressedCharacter = "#";
        buttonPressed = true;
        break;

      case 119: //Button D
        Serial.println("D");
        pressedCharacter = "D";
        buttonPressed = true;
        break;
    }
    buttonTimer = millis();
  }
}