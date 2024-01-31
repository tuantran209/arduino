#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/* Địa chỉ của DS3231 */
const byte DS3231 = 0x68;
/* Số byte dữ liệu sẽ đọc từ DS3231 */
const byte NumberOfFields = 7;
/* khai báo các biến thời gian */
int second, minute, hour, day, wday, month, year;
void setup() {
  Wire.begin();
  /* cài đặt thời gian cho module */
  setTime(11, 17, 00, 4, 24, 5, 23);  // 12:30:45 CN 08-02-2016
  Serial.begin(9600);
}
void loop() {
  /* Đọc dữ liệu của DS3231 */
  readDS3231();
  digitalClockDisplay();
  delay(1000);
}
/*đọc giá trị từ cảm biến */
void readDS3231() {
  Wire.beginTransmission(DS3231);
  Wire.write((byte)0x00);
  Wire.endTransmission();
  Wire.requestFrom(DS3231, NumberOfFields);

  second = bcd2dec(Wire.read() & 0x7f);
  minute = bcd2dec(Wire.read());
  hour = bcd2dec(Wire.read() & 0x3f);  // chế độ 24h.
  wday = bcd2dec(Wire.read());
  day = bcd2dec(Wire.read());
  month = bcd2dec(Wire.read());
  year = bcd2dec(Wire.read());
  year += 2000;
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int bcd2dec(byte num) {
  return ((num / 16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int dec2bcd(byte num) {
  return ((num / 10 * 16) + (num % 10));
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour);
  Serial.print(":");
  Serial.print(minute);
  Serial.print(":");
  Serial.print(second);
  Serial.print(" ");
  Serial.print(day);
  Serial.print("/");
  Serial.print(month);
  Serial.print("/");
  Serial.print(year);
  Serial.println();
}
/* cài đặt thời gian cho DS3231 */
void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr) {
  Wire.beginTransmission(DS3231);
  Wire.write(byte(0x00));  // đặt lại pointer
  Wire.write(dec2bcd(sec));
  Wire.write(dec2bcd(min));
  Wire.write(dec2bcd(hr));
  Wire.write(dec2bcd(wd));  // day of week: Sunday = 1, Saturday = 7
  Wire.write(dec2bcd(d));
  Wire.write(dec2bcd(mth));
  Wire.write(dec2bcd(yr));
  Wire.endTransmission();
}
