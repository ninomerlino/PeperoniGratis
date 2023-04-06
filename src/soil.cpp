#include "soil.h"

SoilMoistureSensor::SoilMoistureSensor(int readingPin){
    this->readingPin = readingPin;
    pinMode(readingPin, INPUT);
    raw = 0;
    moisture = 0;
    minValue = 0;
    maxValue = 4096;
    calibrate();
}

SoilMoistureSensor::SoilMoistureSensor(int readingPin, int minValue, int maxValue){
    this->readingPin = readingPin;
    pinMode(readingPin, INPUT);
    raw = 0;
    moisture = 0;
    this->minValue = minValue;
    this->maxValue = maxValue;
}

void SoilMoistureSensor::calibrate(){
    for(int i=0; i<100000; i++){
        raw = analogRead(readingPin);
        if(raw > maxValue) maxValue = raw;
        if(raw < minValue) minValue = raw;
        moisture = map(raw, minValue, maxValue, 10000, 0)/100;
        Serial.println("[SOIL] Raw value: "+ String(raw)+ " Percentage: "+String(moisture));
        delay(10);
    }
}

float SoilMoistureSensor::getMoisture(){
    raw = analogRead(readingPin);
    moisture = map(raw, minValue, maxValue, 10000, 0)/100;
    Serial.println("[SOIL] Raw value: "+ String(raw)+ " Percentage: "+String(moisture));
    return moisture;
}

float SoilMoistureSensor::getMoisture(int batteryCharge){
    raw = analogRead(readingPin);
    moisture = map(raw, minValue, (batteryCharge*3)/2, 10000, 0)/100;
    Serial.println("[SOIL] Raw value: "+ String(raw)+ ", Battery level: "+String(batteryCharge*3/2)+", Percentage: "+String(moisture));
    return moisture;
}

void SoilMoistureSensor::monitor(int batteryCharge){
    while(1){
        raw = analogRead(readingPin);
        moisture = map(raw, minValue, (batteryCharge*3)/2, 10000, 0)/100;
        Serial.println("[SOIL] Raw value: "+ String(raw)+ ", Battery level: "+String(batteryCharge*3/2)+", Percentage: "+String(moisture));
        delay(1000);
    }
}