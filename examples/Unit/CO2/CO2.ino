#include <M5CoreInk.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <SensirionI2CScd4x.h>

SensirionI2CScd4x scd4x;

void setup() {
    M5.begin();
    M5.Ink.setFont(&FreeSansBold9pt7b);
    M5.Ink.setCursor(50, 30);
    M5.Ink.print("Unit CO2");
    uint16_t error;
    char errorMessage[256];
    M5.Ink.update();
    Wire.begin(32, 33);
    scd4x.begin(Wire);

    // stop potentially previously started measurement
    error = scd4x.stopPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    // Start Measurement
    error = scd4x.startPeriodicMeasurement();
    if (error) {
        Serial.print("Error trying to execute startPeriodicMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

    Serial.println("Waiting for first measurement... (5 sec)");
}

void loop() {
    uint16_t error;
    char errorMessage[256];

    delay(15000);

    // Read Measurement
    uint16_t co2      = 0;
    float temperature = 0.0f;
    float humidity    = 0.0f;
    uint16_t isDataReady  = 0xFF;
    error             = scd4x.getDataReadyStatus(isDataReady);
    if (error) {
        //M5.Lcd.println("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
        return;
    }
    if (!isDataReady) {
        return;
    }
    error = scd4x.readMeasurement(co2, temperature, humidity);
    if (error) {
        //M5.Lcd.println("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else if (co2 == 0) {
        //M5.Lcd.println("Invalid sample detected, skipping.");
    } else {
        M5.Ink.fillScreen(1);
        M5.Ink.setCursor(0, 60);
        M5.Ink.printf("Co2 : %6d ppm\n", co2);
        M5.Ink.printf("Temp:  %.2f C\n", temperature);
        M5.Ink.printf("Humi:  %.2f Rh\n", humidity);
        M5.Ink.update();
    }
}
