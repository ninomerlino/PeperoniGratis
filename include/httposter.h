#include <WiFi.h>
#include <HTTPClient.h>

class Sender{
    private:
        const char* ssid;
        const char* password;
        String serverName;
        void sendGetRequest(String fields);
    public:
        Sender(char* ssid, char* password, String serverName, int port);
        Sender() = default;
        void send(float charge);
        void send(float charge, float temperature);
        void send(float charge, float temperature, float humidity);
        void send(float charge, float temperature, float humidity, float soilMoisture);
};