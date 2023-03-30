#include "httposter.h"

Sender::Sender(char* ssid, char* password, String serverName, int port){
    this->ssid = ssid;
    this->password = password;

    this->serverName = "http://"+serverName+":"+String(port);

    WiFi.begin(this->ssid, this->password);
    Serial.print("[WIFI] Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("[WIFI] Connected to WiFi network with IP Address: " + String(WiFi.localIP()));
}

bool Sender::sendGetRequest(const char *endpoit){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        if(http.begin((serverName+endpoit).c_str())){
            int httpCode = http.GET();
            if(httpCode > 0){
                Serial.println("[HTTP] GET... code: " + String(httpCode));
                return true;
            }else{
                Serial.println("[HTTP] GET... failed, error: " + http.errorToString(httpCode));
                return false;
            }
            http.end();
        }else{
            Serial.println("[HTTP] Unable to connect to server");
            return false;
        }
    }else{
        Serial.println("[HTTP] Not connected to WiFi");
        return false;
    }
}

bool Sender::sendPostRequest(const char *endpoit,const char *payload){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        if(http.begin((serverName+endpoit).c_str())){
            http.addHeader("Content-Type", "application/json");
            int httpCode = http.POST(payload);
            if(httpCode > 0){
                Serial.println("[HTTP] POST... code: " + String(httpCode));
                return true;
            }else{
                Serial.println("[HTTP] POST... failed, error: " + http.errorToString(httpCode));
                return false;
            }
            http.end();
        }else{
            Serial.println("[HTTP] Unable to connect to server");
            return false;
        }
    }else{
        Serial.println("[HTTP] Not connected to WiFi");
        return false;
    }
}
