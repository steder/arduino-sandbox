#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int GPIO_RESET_PIN = 14;

void setupDisplay() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds
}

void displayText(char *text) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(text);
  display.display();
}

void setup() {
  pinMode(GPIO_RESET_PIN, OUTPUT);
  digitalWrite(GPIO_RESET_PIN, LOW);
  Wire.begin(); // begin without an address means "join as master"
  Serial.begin(9600);
  setupDisplay();

  // Try some resets to get the chirp to FUCKING BEHAVE:
  digitalWrite(GPIO_RESET_PIN, HIGH);
  delay(1000);
  digitalWrite(GPIO_RESET_PIN, LOW);
  delay(1000);
  digitalWrite(GPIO_RESET_PIN, HIGH);
  delay(1000);
}

void writeI2CRegister8bit(int addr, int value) {
//  Serial.print("writeI2CRegister8bit: ");
//  Serial.print(addr);
//  Serial.print(", ");
//  Serial.println(value);
  Wire.beginTransmission(addr);
  Wire.write(value);
  Wire.endTransmission();
}


uint8_t readI2CRegister8bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(1100);
  unsigned int available = Wire.requestFrom(addr, 1);
  uint8_t t = Wire.read();
  return t;
}

unsigned int readI2CRegister16bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(1100);
  unsigned int available = Wire.requestFrom(addr, 2);
  unsigned int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

int readI2CRegisterSigned16bit(int addr, int reg) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.endTransmission();
  delay(1100);
  unsigned int available = Wire.requestFrom(addr, 2);
  int t = Wire.read() << 8;
  t = t | Wire.read();
  return t;
}

void loop() {
  // int raw_temp = readI2CRegister16bit(0x18, 5);

  // float c = raw_temp & 0x0FFF; // ignore critical flags
  // c /=  16.0;
  // if (raw_temp & 0x1000) {
  //   c -= 256; // check if negative
  // }

  // float f = c * 9.0 / 5.0 + 32;

  // Serial.print("MCP9808 temp: ");
  // Serial.print(c);
  // Serial.print("*C, ");
  // Serial.print(f);
  // Serial.print("*F");
  // Serial.println();
  
  // MUCK with the MUX
  /*
   Get default value of Analog Mux Register which is:  
  */
  const int AD_READ_MUX_REGISTER = 0x07;
  const int AD_WRITE_MUX_REGISTER = 0x27;
  const int AD_CONTROL_STATUS_REGISTER = 0x06;

  const int AD_CAPACITANCE = 1;

  Serial.print("AD MUX START VALUE: ");
  Serial.println(readI2CRegister8bit(0x20, AD_READ_MUX_REGISTER), HEX);

  Serial.print("AD CONSTROL STATUS VALUE: ");
  Serial.println(readI2CRegister8bit(0x20, AD_CONTROL_STATUS_REGISTER), HEX);
//
//  int thing = readI2CRegister16bit(0x18, 1);
//  Serial.print("thing: ");
//  Serial.print(thing, BIN); 
//  Serial.println();
  
  int cap = readI2CRegister16bit(0x20, 0);
  Serial.print("capacitance: ");
  Serial.println(cap); //read capacitance register
  int chirp_temp = readI2CRegister16bit(0x20, 5);
  Serial.print("Checking temperature: ");
  Serial.println(chirp_temp); //temperature register

  writeI2CRegister8bit(0x20, 3); //request light measurement 
  delay(9000);                   //this can take a while
  int light = readI2CRegisterSigned16bit(0x20, 4);
  Serial.print("light: ");
  Serial.println(light); //read light register

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // display.print("temp: ");
  // display.print(c);
  // display.print("*C, ");
  // display.print(f);
  int reset = digitalRead(GPIO_RESET_PIN);
  display.print("RESET: ");
  display.print(reset);
  display.print("*F, Capacitance: ");
  display.print(cap);
  display.print(", Temp: ");
  display.print(chirp_temp);
  display.print(", Light: ");
  display.println(light);
  display.display();
  
  delay(500);
}
