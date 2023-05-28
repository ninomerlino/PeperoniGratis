#include "util.h"
#include "time.h"

class Pump{
    private:
        int pin;
        int time;
        int threshold;

        char* ntpServer;
        long  gmtOffset_sec;
        int   daylightOffset_sec;

        struct tm timeinfo;

        int last_day;
        int last_hour;

    public:
        Pump() = default;
        Pump(int pin);
        Pump(int pin, int time, int threshold);
        void irrigate(int moisture);
};