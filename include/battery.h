#ifndef __BATTERY__
#define __BATTERY__
#include "util.h"

class Battery{
    private:
        int readingPin;
        float charge;
        float raw;

        float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
    public:
        Battery(int readingPin);
        Battery() = default;
        ~Battery();	
        float getCharge();
};
#endif