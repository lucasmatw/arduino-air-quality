#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>

// ADIDAX
#include <dht11.h>
#define DHT11PIN 2
dht11 DHT11;

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
#define OLED_RESET 4  // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define sensor A0

int gasLevel = 0;  //int variable for gas level
String quality = "";


void readDHTSensor() {
  int chk = DHT11.read(DHT11PIN);

  // Serial.print("Humidity (%): ");
  float h = (float)DHT11.humidity;

  // Serial.print("Temperature  (C): ");
  float t = (float)DHT11.temperature;

  Serial.print("Humedad: ");
  Serial.println(h);
  Serial.print("Temp: ");
  Serial.println(t);
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed  to read from DHT sensor!");
    return;
  }

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setFont();
  display.setCursor(0, 43);
  display.println("Temp  :");
  display.setCursor(80, 43);
  display.println(t);
  display.setCursor(114, 43);
  display.println("C");
  display.setCursor(0, 56);
  display.println("HR    :");
  display.setCursor(80, 56);
  display.println(h);
  display.setCursor(114, 56);
  display.println("%");

}

void readGasSensor() {
  gasLevel = analogRead(sensor);

  Serial.print("Gas level: ");
  Serial.println(gasLevel);

  if (gasLevel < 181) {
    quality = " Buena";
  } else if (gasLevel > 181 && gasLevel < 225) {
    quality = " Mala";
  } else if (gasLevel > 225 && gasLevel < 300) {
    quality = " Muy mala";
  } else if (gasLevel > 300 && gasLevel < 350) {
    quality = " Toxica";
  } else {
    quality = " Fatal";
  }


  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(1, 5);
  display.println("Calidad:");
  display.setTextSize(1);
  display.setCursor(10, 23);
  display.setFont(&FreeMonoOblique9pt7b);
  display.print(quality);
  display.setFont();
  display.print(" (");
  display.print(gasLevel);
  display.print(")");
}

void twoSentenceDisplay(int sizeOne, char* textOne, int sizeTwo, char* textTwo) {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(sizeOne);
  display.setCursor(1, 5);
  display.println(textOne);

  display.setTextSize(sizeTwo);
  display.setCursor(1, 31);
  display.println(textTwo);
  display.display();
  delay(2000);
}

void setup() {
  Serial.begin(9600);
  pinMode(sensor, INPUT);
  if (!display.begin(i2c_Address, true)){ // Address 0x3C default
    Serial.println(F("SSD1306 allocation failed"));
  }

  twoSentenceDisplay(2, "Air", 1, "Quality monitor");
  twoSentenceDisplay(1, "Alvaro Piorno", 1, "Lucas Matwiejczuk");
  twoSentenceDisplay(2, "Arduino", 3, "UNQ");
}

void loop() {
  display.clearDisplay();
  readGasSensor();
  readDHTSensor();
  display.display();
  delay(500);
}

