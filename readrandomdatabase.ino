#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define FIREBASE_HOST "hostid"
#define FIREBASE_AUTH "secret code"
const char *ssid = "ssid";     // Change this to your WiFi SSID
const char *password = "password"; // Change this to your WiFi password

String codes[10]; // Assuming you have 10 codes (code1 to code10)
unsigned long previousMillis = 0; // Variable to store the last time a code was printed
const long interval = 5000; // Interval between code prints (5 seconds)

const int irSensorPin = D1; // Pin connected to the IR sensor

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // Read codes from Firebase and store them into the array
  readCodes();

  // Initialize IR sensor pin
  pinMode(irSensorPin, INPUT);
}

void loop() {
  // Check if it's time to print a new random code
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a code was printed
    previousMillis = currentMillis;

    // Check if the IR sensor signal is low
    if (digitalRead(irSensorPin) == LOW) {
      // Print a random code
      printRandomCode();
    }
  }
}

void readCodes() {
  // Read codes from Firebase
  for (int i = 0; i < 10; i++) {
    String codePath = "codesSet/code" + String(i + 1); // Construct path to each code
    FirebaseData fbdo;                                 // Create FirebaseData object
    if (Firebase.getString(fbdo, codePath)) {         // Read code value from Firebase
      codes[i] = fbdo.to<String>();                   // Store code value into array
    } else {
      Serial.println("Error reading code " + String(i + 1));
    }
  }
}

void printRandomCode() {
  // Generate a random index
  int randomIndex = random(0, 10);

  // Print the code at the random index
  Serial.print("Randomly chosen code: ");
  Serial.println(codes[randomIndex]);
}
