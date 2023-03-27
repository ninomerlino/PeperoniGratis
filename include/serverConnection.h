#ifndef __INFLUX__
#define __INFLUX__
#include "util.h"
#include "logger.h"
#include <InfluxDbClient.h>

class InfluxServer{
    private:
        InfluxDBClient client;
    public:
        InfluxServer();
        void uploadPoint(Point &point);
        void connectToWifi(uint64_t timeout = 36000);
        void connectToInfluxDB(uint64_t timeout = 36000);
};
#endif