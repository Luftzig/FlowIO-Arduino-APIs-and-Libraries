#include <Module16Inputs.h>
#include <bluefruit.h>

Module16Inputs module16inputs;
uint16_t adcValues[16];

void setup() {
    module16inputs = Module16Inputs();
    Serial.begin(115200);
}

void loop() {
    module16inputs.read16ChannelsInto(adcValues);
    for (int i = 0; i < 16; i++) {
        Serial.print(adcValues[i]);
        Serial.print("\t");
    }
    Serial.println();

    //To read a single channel, use the method .readChannel(#)
    //Reading the channels individually is more inefficient than reading
    //them all at once, because there are many more operations happening.
}
