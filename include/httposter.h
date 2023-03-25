#include <WiFi.h>
#include <HTTPClient.h>

class Sender {
private:
    const char* ssid;
    const char* password;
    String serverName;
public:
    Sender(char* ssid, char* password, String serverName, int port);
    Sender() = default;
    bool sendPostRequest(const char* endpoit, const char* payload);
    bool sendGetRequest(const char* endpoit);
};