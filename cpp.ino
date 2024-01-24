#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

// Set the I2C address of your LCD module
int lcdAddress = 0x27; // Change this according to your LCD module

// Set the number of columns and rows of your LCD module
int lcdColumns = 16;
int lcdRows = 2;

// Create an instance of the LiquidCrystal_I2C class
LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows);

// Create an instance of the MFRC522 class
MFRC522 mfrc522(10, 9); // SDA, RST

// Create an instance of SoftwareSerial for the new RX pin
SoftwareSerial mySerial(2, 1); // RX, TX

// Function prototype
String getCardUID();

void setup() {
  Wire.begin();
  mySerial.begin(9600); // Use the baud rate suitable for your application
  Serial.begin(9600);   // Use the baud rate suitable for your application
  while (!Serial);       // Leonardo: wait for Serial Monitor

  // Initialize the LCD
  lcd.begin(lcdColumns, lcdRows);
  lcd.backlight(); // Turn on the backlight

  Serial.println("\nI2C Scanner and RFID Reader");

  SPI.begin();      // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
}

void loop() {
  // RFID Reader part
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Place a card...");

  Serial.println("Place a card...");

  // Check if a card is present and read its UID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Card UID:");
    lcd.setCursor(0, 1);
    String uid = getCardUID();
    lcd.print(uid);

    Serial.print("Card UID: ");
    Serial.println(uid);

    // Send UID to ESP8266 over SoftwareSerial
    mySerial.println(uid);

    // Wait for ESP8266 response (access granted/denied) - handle this in ESP8266 code
    delay(10000); // Adjust the delay as needed
  }

  delay(500); // Small delay for stability
}

// Function to get the card UID
String getCardUID() {
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  return cardUID;
}
