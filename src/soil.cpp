#include "soil.h"

SoilMoistureSensor::SoilMoistureSensor(int readingPin){
    this->readingPin = readingPin;
    raw = 0;
    moisture = 0;
}

SoilMoistureSensor::~SoilMoistureSensor(){
}

