#include <Wire.h>
#include <WiFi.h>


const int GPIO_RESET_PIN = 14;
// This chip only has a 2.4GHz radio so only 2.4GHz networks will work
const char* wifi_ssid = "2.4-Portlandia";
const char* wifi_password = "rulefromorbit";

//WiFiServer server(80);

// Bluetooth:

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID        "9d0fda08-6347-4f9d-9c57-da70a0b6b8cd"
#define CHARACTERISTIC_UUID "1e1aee34-f68a-4758-9a9c-5bd7f5d1eb8d"

char bluetoothinfo[256] = "Hello Bluetooth World!";

class OurBLECallbacks : public BLECharacteristicCallbacks {
public:
  void onRead(BLECharacteristic* pCharacteristic) {
    Serial.println("Someone read our BLE data!");
  }
  void onWrite(BLECharacteristic* pCharacteristic) {
    Serial.println("Someone wrote our BLE data!");
    strcpy(bluetoothinfo, pCharacteristic->getValue().c_str());
    Serial.print("bluetooth info: ");
    Serial.println(bluetoothinfo);
  }
};

void writeI2CRegister8bit(int addr, int value) {
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}

unsigned int readI2CRegister16bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  // Wire.endTransmission();
  delay(1000);
  Wire.requestFrom(addr, 2);
  // Wire.available()    // slave may send less than requested
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

void setup() {
  Wire.begin();
  Serial.begin(9600);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  writeI2CRegister8bit(0x20, 6); //reset

  // So we wired a GPIO pin and we need to set to high to take the chirp out of reset mode:
  pinMode(GPIO_RESET_PIN, OUTPUT);

//  WiFi.begin(wifi_ssid, wifi_password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//
//  Serial.println(F("WiFi connected"));
//  Serial.println(F("IP address: "));
//  Serial.println(WiFi.localIP());

  // Print WiFi MAC address:
  //printMacAddress();

  // web server setup:
  //server.begin();

  Serial.println(F("Starting BLE work!"));

  BLEDevice::init("Chicken");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
  OurBLECallbacks *callbacks = new OurBLECallbacks();

  pCharacteristic->setValue(bluetoothinfo);
  pCharacteristic->setCallbacks(callbacks);
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println(F("Characteristic defined! Now you can read it in your phone!"));
}

void loop() {
//  digitalWrite(GPIO_RESET_PIN, HIGH);
//  Serial.print("Checking capacitance: ");
//  Serial.println(readI2CRegister16bit(0x20, 0)); //read capacitance register
//  Serial.print("Checking temperature: ");
//  Serial.println(readI2CRegister16bit(0x20, 5)); //temperature register
//  Serial.print("Checking light: ");
//  writeI2CRegister8bit(0x20, 3); //request light measurement 
//  Serial.println(readI2CRegister16bit(0x20, 4)); //read light register

//  // scan for existing networks:
//  Serial.println("Scanning available networks...");
//  listNetworks();
//  delay(10000);

//  Serial.println(F("WiFi connected"));
//  Serial.println(F("IP address: "));
//  Serial.println(WiFi.localIP());

  Serial.print("bluetooth storage: ");
  Serial.println(bluetoothinfo);

  delay(10000);
  // Print WiFi MAC address:
  //printMacAddress();

  //httpServer();
}

/* void printMacAddress() {
  // the MAC address of your Wifi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
} */

/* void httpServer() {
  WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);               // GET /H turns the LED on
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);                // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
 */
