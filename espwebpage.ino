#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

const char* ssid = "MyWifi";
const char* password = "Vikas@16";
const char* apiUrl = "https://attendance-53womng7n-ratnajeet00.vercel.app/checkDatabase";

SoftwareSerial mySerial(2, 1); // RX, TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}
void loop() {
  // Read RFID UID from Arduino over SoftwareSerial
  if (mySerial.available() > 0) {
    String uid = mySerial.readStringUntil('\n');

    // Create a WiFiClient object
    WiFiClient wifiClient;

    // Make HTTP POST request to the backend API
    HTTPClient http;
    http.begin(wifiClient, apiUrl); // Pass the WiFiClient object to begin()
    http.addHeader("Content-Type", "application/json");

    // Prepare the JSON payload
    String jsonPayload = "{\"uid\":\"" + uid + "\"}";

    int httpCode = http.POST(jsonPayload);

    if (httpCode == HTTP_CODE_OK) {
      String response = http.getString();
      Serial.println("Server response: " + response);
      // Process the server response (access granted/denied) as needed
    } else {
      Serial.println("HTTP request failed");
    }

    http.end();
  }

  delay(5000); // Adjust the delay as needed
}
