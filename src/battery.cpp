#include "battery.h"

Battery::Battery(int readingPin){
    this->readingPin = readingPin;
    raw = 0;
    charge = 0;
}

Battery::~Battery(){
}

float Battery::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float Battery::getCharge()
{
    raw = analogRead(34);
    Serial.printf("Raw value %f \n", raw);
    raw = raw / 4095.0 * 3.3;
    Serial.printf("Caluclated value %f \n", raw);
    raw = mapfloat(raw, 0.0, 2.8, 0, 100);
    Serial.printf("Mapped value %f \n", raw);
    Serial.println(" ");
    return raw;
}