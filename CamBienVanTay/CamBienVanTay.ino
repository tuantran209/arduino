#include <ESP8266WiFi.h>
#include <MQTT.h>
#include <FirebaseESP8266.h>
#include <Adafruit_Fingerprint.h>
#include <string.h>

const char* ssid = "VIETTEL LAB";
const char* password = "kvt1ptit";
uint8_t id;

#define FIREBASE_HOST "https://test-7777d-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "iyFjiGJ37BDiApCSmnEeO9rYTGe4mzgKbMz2BAXK"

SoftwareSerial mySerial(D2, D1);
WiFiClient net;
MQTTClient client;
FirebaseData fbdo;
bool isAdmin = false;
const char* passAdmin = "Hi123"; 
  
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// Khai báo mảng tra cứu
String userTable[][2] = {
  {"Vuong", "1"},
  {"Yen", "2"},
  {"Tuan", "3"},
  {"Bich", "4"},
  {"thayChung", "5"}
};

String getUserNameById(String id) {
  const char* idChar = id.c_str(); // Chuyển đổi kiểu dữ liệu từ String sang char*
  int numUsers = sizeof(userTable) / sizeof(userTable[0]);
  for (int i = 0; i < numUsers; i++) {
    if (strcmp(userTable[i][1].c_str(), idChar) == 0) { // So sánh giá trị trong mảng với ID nhập từ Serial
      return userTable[i][0]; // Trả về tên người dùng
    }
  }
  return "-1"; // Không tìm thấy người dùng
}




void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }
}

String readnumber(void) {
  String num = "";
  while (num == "") {
    while (! Serial.available());
    num = Serial.readStringUntil('\n');
  }
  return num;
}

uint8_t getFingerprintEnroll() {

  int p = -1;
  Serial.print("Waiting for valid finger to enroll as #"); Serial.println(id);
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("ID "); Serial.println(id);
  p = -1;
  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  Serial.print("Creating model for #");  Serial.println(id);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  Serial.print("ID "); Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
    return 0;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}

void loop() {
  Serial.println("Enter password:"); // yêu cầu người dùng nhập mật khẩu
  while (!Serial.available()) {
    delay(1);
  }
  String password = Serial.readStringUntil('\n');

  if (password == passAdmin) { // Nếu mật khẩu đúng
    Serial.println("Admin access granted. Please enter user ID:");

    while (!Serial.available()) {
      delay(1);
    }
    String id = readnumber(); // Nhập ID

    String userName = getUserNameById(id);
    if (userName != "-1") { // Nếu tìm thấy người dùng
      Serial.println(id);
  
  while (!  getFingerprintEnroll() );
      Serial.println("User found: " + userName);
      Serial.println("Please enter user's full name:");
      while (!Serial.available()) {
        delay(1);
      }
      String fullName = Serial.readStringUntil('\n');
      Serial.println("Please enter user's class:");
      while (!Serial.available()) {
        delay(1);
      }
      String userClass = Serial.readStringUntil('\n');
      Serial.println("Please enter user's birthdate (DD/MM/YYYY):");
      while (!Serial.available()) {
        delay(1);
      }
      String birthdate = Serial.readStringUntil('\n');

      // Gửi dữ liệu mới lên Firebase
      Firebase.setString(fbdo, "users/" + userName + "/fullName", fullName);
      Firebase.setString(fbdo, "users/" + userName + "/class", userClass);
      Firebase.setString(fbdo, "users/" + userName + "/birthdate", birthdate);
      // Firebase.setInt(fbdo, "users/" + userName + "/fingerprintID", fingerprintID);
      // if (Firebase.failed()) {
      //   Serial.print("Setting /users/" + userName + " failed:");
      //   Serial.println(fbdo.errorReason());
      //   return;
      // }
      Serial.println("User data stored in database.");
    } else { // Nếu không tìm thấy người dùng
      Serial.println("User not found. Please try again in 3 seconds.");
      delay(3000); // Đợi 3 giây
    }
  } else { // Nếu mật khẩu sai
    Serial.println("Access denied.");
  }
}
