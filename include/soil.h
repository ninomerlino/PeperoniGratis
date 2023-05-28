#include "util.h"

class SoilMoistureSensor {
  private:
    int readingPin;
    int minValue;
    int maxValue;
    int raw;
    float moisture;

    void calibrate();
  public:
    SoilMoistureSensor(int readingPin);
    SoilMoistureSensor(int readingPin, int minValue, int maxValue);
    SoilMoistureSensor() = default;
    float getMoisture();
    float getMoisture(int rawBattery);
    void monitor(int batteryCharge);
};