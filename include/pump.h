#include "util.h"

class Pump{
    private:
        int pin;
        int time;
        int threshold;
    public:
        Pump() = default;
        Pump(int pin);
        Pump(int pin, int time, int threshold);
        void irrigate(int moisture);
};