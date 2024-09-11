#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>

#define SS_PIN 5
#define RST_PIN 22
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define RELAY_PIN 15

const char* net = "Wypełnij";
const char* pass = "Wypełnij!";

String allowedUIDs[] = {"12345678", "87654321"}; 

void unlockDoor() {
  digitalWrite(RELAY_PIN, HIGH); 
  delay(5000);                   
  digitalWrite(RELAY_PIN, LOW); 
}

void setup() {
  Serial.begin(115200);
  SPI.begin();           
  mfrc522.PCD_Init();   
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  Serial.println("Laczenie z WiFi...");
  WiFi.begin(net, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Laczenie...");
  }
  Serial.println("Polaczono z WiFi!");

  Serial.println("Gotowy do skanowania RFID.");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += String(mfrc522.uid.uidByte[i], HEX);
  }
  uid.toUpperCase(); 
  Serial.println("Wykryto UID: " + uid);

  for (String allowedUID : allowedUIDs) {
    if (uid == allowedUID) {
      Serial.println("Dostęp przyznany!");
      unlockDoor();  
      break;
    }
  }

  mfrc522.PICC_HaltA();
}

//unlockDoor(): Otwiera zamek przez aktywację przekaźnika na 5 sekund, a następnie zamyka zamek.
//setup(): Inicjalizuje moduł RFID i WiFi oraz ustawia pin przekaźnika jako wyjście. Łączy się z siecią WiFi i wyświetla komunikaty informujące o statusie połączenia.
//loop(): Główna logika programu. Sprawdza, czy wykryto nową kartę RFID i odczytuje jej UID. Jeśli UID pasuje do jednego z dozwolonych na liście allowedUIDs, otwiera zamek.

