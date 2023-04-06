#include "pump.h"

Pump::Pump(int pin){
    this->pin = pin;
    this->time = 30000;
    this->threshold = 50;
    pinMode(pin, OUTPUT);
}

void Pump::irrigate(int moisture){
    if(moisture < threshold){
        Serial.println("[PUMP] Irrigating");
        digitalWrite(pin, HIGH);
        delay(time);
        digitalWrite(pin, LOW);
    }
}