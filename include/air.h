#include "util.h"
#include <dht.h>

class AirSensor{
    
    private:
        int pin;
        float temperature;
        float humidity;
        DHT sensor = DHT(AIR_PIN, DHT11);
    public:
        AirSensor(uint8_t pin);
        AirSensor() = default;
        float getTemperature();
        float getHumidity();
};