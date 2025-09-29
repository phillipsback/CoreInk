#include <CoreInk.h>

#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"
SHT3X sht30;
Adafruit_BMP280 bme;

float tmp      = 0.0;
float hum      = 0.0;
float pressure = 0.0;

void setup() {
    M5.begin(true, true); 
    M5.Ink.InitClib(GxEPD_BLACK, GxEPD_WHITE);
    Wire.begin(25, 26);
    while(!bme.begin(0x76)){
      M5.Ink.cSimPrint("找不到可用BMP280感測器，請確認配線!", 3, 35);
      M5.Ink.update();
    }
}

void loop() {
    M5.Ink.clear();
    pressure = bme.readPressure();
    sht30.get();
    tmp = sht30.cTemp; 
    hum = sht30.humidity;
    M5.Ink.setCursor(0, 20);
    M5.Ink.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n",
                  tmp, hum, pressure);
    M5.Ink.update();
    delay(15000);
}
