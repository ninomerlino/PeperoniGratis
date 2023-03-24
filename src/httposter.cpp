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
    Serial.print("[WIFI] Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}

void Sender::send(float charge){
    String serverCharge = serverName+"/update-data?charge="+String(charge);
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        if(http.begin(serverCharge.c_str())){
            int httpCode = http.GET();

            if(httpCode > 0){
                Serial.println("[HTTP] GET... code: " + String(httpCode));
            }else{
                Serial.println("[HTTP] GET... failed, error: " + http.errorToString(httpCode));
            }

            http.end();
        }else{
            Serial.println("[HTTP] Unable to connect to server");
        }
    }else{
        Serial.println("[HTTP] Not connected to WiFi");
    }
}