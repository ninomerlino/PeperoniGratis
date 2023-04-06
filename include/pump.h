#include "util.h"

class Pump{
    private:
        int pin;
        int time;
        int threshold;
    public:
        Pump() = default;
        Pump(int pin);
        void irrigate(int moisture);
};