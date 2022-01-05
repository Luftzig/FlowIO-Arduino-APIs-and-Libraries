#include <bluefruit.h>
#include <Arduino.h>
#include <array>
#include <numeric>
#include <cmath>

#define SIGNAL A3
#define S0 0
#define S1 1
#define S2 2
#define S3 19

constexpr uint16_t NUM_SAMPLES = 100;

using std::array;

array <uint16_t, NUM_SAMPLES> samples;

uint8_t selectedChannel = -1;
unsigned long startSampleTime;
unsigned long endSampleTime;

void setup() {
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);
    pinMode(SIGNAL, INPUT);
    analogReadResolution(12);
}

void loop() {
    while (!Serial) {
        return;
    }

    if (Serial.available() > 0) {
        selectedChannel = ((unsigned short) Serial.parseInt() - 1);
    }

    if (selectedChannel >= 16) {
        return;
    }
    digitalWrite(S0, bitRead(selectedChannel, 0));
    digitalWrite(S1, bitRead(selectedChannel, 1));
    digitalWrite(S2, bitRead(selectedChannel, 2));
    digitalWrite(S3, bitRead(selectedChannel, 3));

    delayMicroseconds(250);
    startSampleTime = micros();
    for (auto &sample : samples) {
        sample = analogRead(SIGNAL);
    }
    endSampleTime = micros();

    auto avg = std::accumulate(samples.begin(), samples.end(), 0) / NUM_SAMPLES;
    auto dev = pow(
            std::accumulate(samples.begin(), samples.end(), 0,
                            [&](double acc, double x) { return acc + pow(avg - x, 2); }
            ) / NUM_SAMPLES,
            0.5);

//    for (const auto &sample : samples) {
//        Serial.print(sample);
//        Serial.print("\t");
//    }
    Serial.print(avg);
    Serial.print("\t");
    Serial.print(dev);
    Serial.print("\t");
    Serial.print(endSampleTime - startSampleTime);
    Serial.println("micros");
}
