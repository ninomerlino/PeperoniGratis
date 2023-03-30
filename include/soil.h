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
    float getMoisture();
};