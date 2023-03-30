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
    Serial.println("[BATT] Raw value: "+ String(raw));
    raw = raw / 4095.0 * 3.3;
    Serial.println("[BATT] Caluclated value: "+String(raw));
    charge = mapfloat(raw, 0.0, 2.8, 0, 100);
    Serial.println("[BATT] Mapped value: "+String(charge));
    return charge;
}