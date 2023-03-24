#include <WiFi.h>
#include <HTTPClient.h>

class Sender{
    private:
        const char* ssid;
        const char* password;
        String serverName;
        bool sendPostRequest(const char *endpoit,const char *payload);
    public:
        Sender(char* ssid, char* password, String serverName, int port);
        Sender() = default;
        bool send(float* charge=nullptr, float* temperature=nullptr, float* humidity=nullptr, float* soilMoisture=nullptr);
};