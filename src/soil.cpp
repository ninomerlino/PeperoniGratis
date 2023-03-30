#include "soil.h"

SoilMoistureSensor::SoilMoistureSensor(int readingPin){
    this->readingPin = readingPin;
    raw = 0;
    moisture = 0;
    minValue = 0;
    maxValue = 4096;
}

SoilMoistureSensor::SoilMoistureSensor(int readingPin, int minValue, int maxValue){
    this->readingPin = readingPin;
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
        delay(10);
    }
}

float SoilMoistureSensor::getMoisture(){
    raw = analogRead(readingPin);
    moisture = map(raw, minValue, maxValue, 10000, 0)/100;;
    return moisture;
}