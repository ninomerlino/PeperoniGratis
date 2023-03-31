#include "air.h"

AirSensor::AirSensor(uint8_t pin){
    this->pin = pin;
    this->temperature = 0;
    this->humidity = 0;
}

float AirSensor::getTemperature(){
    temperature = sensor.readTemperature();
    Serial.println("[AIR ] Temperature: " + String(temperature));
    return temperature;
}

float AirSensor::getHumidity(){
    humidity = sensor.readHumidity();
    Serial.println("[AIR ] Humidity: " + String(humidity));
    return humidity;
}
