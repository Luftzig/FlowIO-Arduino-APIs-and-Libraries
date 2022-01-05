#include <Arduino.h>
#include <bluefruit.h>
#include <Module16Inputs.h>
#include "Adafruit_NeoPixel.h"

Module16Inputs sensors;
typedef unsigned long ulong;
ulong lastTimestamp;

#define neoPixelPin 8
Adafruit_NeoPixel led;
uint16_t values[16] = {0};

void setup() {
    analogReadResolution(12);
    sensors = Module16Inputs();
    Serial.begin(115200);
    lastTimestamp = millis();
}

void pixel(uint8_t r, uint8_t g, uint8_t b) {
    Adafruit_NeoPixel _thePixel(1, neoPixelPin); //1 pixel on pin 8
    _thePixel.begin();
    // _thePixel.setPixelColor(0, _thePixel.Color(0,0,0));
    _thePixel.show(); //If we don't have this show() we get a bad result the first time pixel() is invoked.
    //We can put this either here or after the next line, but we must have 2 of the .show() functions.
    _thePixel.setPixelColor(0, _thePixel.Color(r, g, b));
    _thePixel.show(); //Having the second show() overrides the bad result with the correct color.
}

struct PrintConfig {
    const char *separator;
    const char *suffix;
    const char *prefix;
} defaultPrintConfig = {"\t", "\n", ""};

template<typename T>
void print(Stream &sout, T values[], size_t length, PrintConfig config = defaultPrintConfig) {
    sout.print(config.prefix);
    for (size_t i = 0; i < length; i++) {
        sout.print(values[i]);
        if (i < length - 1) {
            sout.print(config.separator);
        }
    }
    sout.print(config.suffix);
}

void loop() {

    while (!Serial) {
        pixel(50, 0, 5);
        return;
    }

    pixel(0, 50, 5);

    ulong currentTime = millis();
    if (currentTime - lastTimestamp < 50) {
        return;
    } else {
        for (int i = 1; i <= 16; i++) {
            values[i - 1] = sensors.readChannel(i);
        }
        Serial.print(currentTime);
        Serial.print("\t");
        print(Serial, values, 16);
    }
}
