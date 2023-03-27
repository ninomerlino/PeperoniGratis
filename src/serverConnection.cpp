#include "serverConnection.h"

extern Logger logger;

InfluxServer::InfluxServer(){
    this->client = InfluxDBClient(INFLUXDB_URL_MACRO, INFLUXDB_ORG_MACRO, INFLUXDB_BUCKET_MACRO, INFLUXDB_TOKEN_MACRO);
}

void InfluxServer::connectToWifi(uint64_t timeout){
  uint64_t start = millis();
  WiFi.begin(SSID_MACRO, PASSWORD_MACRO);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    if(millis() - start > timeout){
      logger.log("Wifi connection timeout", ERROR);
      return;
    }
  }
  logger.log(WiFi.localIP(), INFO);
}

void InfluxServer::connectToInfluxDB(uint64_t timeout){
  uint64_t start = millis();
  while(!client.validateConnection()){
    delay(500);
    Serial.print(".");
    if(millis() - start > timeout){
      logger.log("InfluxDB connection timeout", ERROR);
      logger.log(client.getLastErrorMessage(), ERROR);
      return;
    }
  }
}

void InfluxServer::uploadPoint(Point &point){
  if(!WiFi.isConnected()){
    logger.log("Wifi connection lost", ERROR);
    connectToWifi();
  }
  if(!client.validateConnection()){
    logger.log("InfluxDB connection lost", ERROR);
    connectToInfluxDB();
  }

  if (!client.writePoint(point)) {
    logger.log("InfluxDB write failed: ", ERROR);
    logger.log(client.getLastErrorMessage(), ERROR);
  }
}