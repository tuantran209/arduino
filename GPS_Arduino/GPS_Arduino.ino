#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3; // Chân RX và TX của Arduino
static const uint32_t GPSBaud = 9600;
static const int timeZone = 7; // Múi giờ số 7 (GMT+7)

TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop()
{
  while (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      displayInfo();
    }
  }

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Time: "));
  if (gps.time.isValid())
  {
    int hour = gps.time.hour() + timeZone;
    if (hour < 0)
    {
      hour += 24;
    }
    else if (hour >= 24)
    {
      hour -= 24;
    }

    if (hour < 10)  Serial.print(F("0"));
    Serial.print(hour);

    if (gps.time.minute() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));

    if (gps.time.second() < 10)
      Serial.print(F("0"));
    Serial.print(gps.time.second());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}