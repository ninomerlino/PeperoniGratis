#include "httposter.h"
#include "logger.h"

extern Logger logger;

Sender::Sender(char* ssid, char* password, String serverName, int port){
    this->ssid = ssid;
    this->password = password;

    this->serverName = "http://"+serverName+":"+String(port);

    WiFi.begin(this->ssid, this->password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        logger.log("[WIFI] Connecting to WiFi..", INFO);
    }
    logger.log("[WIFI] Connected to WiFi network with IP Address: " + WiFi.localIP(), INFO);
}

bool Sender::sendPostRequest(const char *endpoit,const char *payload){
    if(WiFi.status() == WL_CONNECTED){
        HTTPClient http;
        if(http.begin((serverName+endpoit).c_str())){
            http.addHeader("Content-Type", "application/json");
            int httpCode = http.POST(payload);
            if(httpCode > 0){
                logger.log("[HTTP] POST... code: " + String(httpCode));
                return true;
            }else{
                logger.log("[HTTP] POST... failed, error: " + http.errorToString(httpCode), ERROR);
                return false;
            }
            http.end();
        }else{
            logger.log("[HTTP] Unable to connect to server", ERROR);
            return false;
        }
    }else{
        logger.log("[HTTP] Not connected to WiFi", ERROR);
        return false;
    }
}

bool Sender::send(Option<float> charge, Option<float> temperature, Option<float> humidity, Option<float> soilMoisture){
    String payload = "{";
    if(charge.isSet()){
        payload += "\"charge\":"+String(charge.get())+",";
    }
    if(temperature.isSet()){
        payload += "\"temperature\":"+String(temperature.get())+",";
    }
    if(humidity.isSet()){
        payload += "\"humidity\":"+String(humidity.get())+",";
    }
    if(soilMoisture.isSet()){
        payload += "\"soilMoisture\":"+String(soilMoisture.get())+",";
    }
    payload.remove(payload.length()-1,1);
    payload += "}";
    return sendPostRequest("/update-data",payload.c_str());
}
