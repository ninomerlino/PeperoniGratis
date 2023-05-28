#include "pump.h"

Pump::Pump(int pin){
    this->pin = pin;
    this->time = 15000;
    this->threshold = 75;
    pinMode(pin, OUTPUT);
    gmtOffset_sec = 3600;
    daylightOffset_sec = 0;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

Pump::Pump(int pin, int time, int threshold){
    this->pin = pin;
    this->time = time*1000;
    this->threshold = threshold;
    pinMode(pin, OUTPUT);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
}

void Pump::irrigate(int moisture){
    if(moisture < threshold){
        Serial.println("[PUMP] Irrigating");
        digitalWrite(pin, HIGH);
        delay(time);
        digitalWrite(pin, LOW);
    }
}