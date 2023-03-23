#include "httposter.h"

Sender::Sender(char* ssid, char* password, String serverName, int port){
    this->ssid = ssid;
    this->password = password;

    this->serverName = "http://"+serverName+":"+String(port);

    WiFi.begin(this->ssid, this->password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("[WIFI] Connecting to WiFi..");
    }
    Serial.println("[WIFI] Connected to the WiFi network");
}

void Sender::send(){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        http.begin(this->serverName);
        int httpCode = http.GET();
        if(httpCode > 0){
            String payload = http.getString();
            Serial.println(payload);
        }else{
            Serial.println("[HTTP] GET... failed, error: " + http.errorToString(httpCode));
        }
        http.end();
    }
}