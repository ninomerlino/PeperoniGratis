#include "util.h"
#include "logger.h"
#include <InfluxDbClient.h>

class InfluxServer{
    private:
        Logger *logger;
        InfluxDBClient client;
        void connectToWifi(uint64_t timeout = 1000);
        void connectToInfluxDB(uint64_t timeout = 10000);
    public:
        InfluxServer(Logger* logger);
        InfluxServer() = default;
        void uploadPoint(Point point);
};