#include <WiFi.h>
#include <HTTPClient.h>
#include "optional.h"

class Sender {
private:
    const char* ssid;
    const char* password;
    String serverName;
    bool sendPostRequest(const char* endpoit, const char* payload);
public:
    Sender(char* ssid, char* password, String serverName, int port);
    Sender() = default;
    bool send(Option<float> charge, Option<float> temperature = Option<float>(), Option<float> humidity = Option<float>(), Option<float> soilMoisture = Option<float>());
};