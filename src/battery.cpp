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
    //3200 = 4V

    raw = analogRead(readingPin);
    Serial.println("[BATT] Raw value: "+ String(raw));

    float readVoltage = (raw / 4095.0) * 5.12;

    Serial.println("[BATT] Calculated voltage: "+String(readVoltage));
    charge = mapfloat(readVoltage, 3.0, 4.0, 0, 100);
    Serial.println("[BATT] Percentage: "+String(charge));
    return charge;
}

int Battery::getRaw()
{
    raw = analogRead(readingPin);
    return raw;
}

float Battery::getVoltage()
{
    raw = (float) analogRead(readingPin);
    return (float) (raw / 4095.0) * 5.12;
}