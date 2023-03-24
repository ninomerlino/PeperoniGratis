#include "util.h"

class SoilMoistureSensor {
  public:
    SoilMoistureSensor(int readingPin);
    float getMoisture();
  private:
    int readingPin;
    float raw;
    float moisture;
};