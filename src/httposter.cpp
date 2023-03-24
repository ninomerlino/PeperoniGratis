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

bool Sender::send(float* charge=nullptr, float* temperature=nullptr, float* humidity=nullptr, float* soilMoisture=nullptr){
    String payload = "{";
    if(charge != nullptr)
        payload += "\"charge\":"+String(*charge)+",";
    if(temperature != nullptr)
        payload += "\"temperature\":"+String(*temperature)+",";
    if(humidity != nullptr)
        payload += "\"humidity\":"+String(*humidity)+",";  
    if(soilMoisture != nullptr)
        payload += "\"soilMoisture\":"+String(*soilMoisture)+",";
    payload.remove(payload.length()-1,1);
    payload += "}";
    return sendPostRequest("/update-data",payload.c_str());
}
