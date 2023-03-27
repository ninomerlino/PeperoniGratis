#include "serverConnection.h"

InfluxServer::InfluxServer(Logger* logger){
    this->logger = logger;
    this->client = InfluxDBClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
}

void InfluxServer::connectToWifi(uint64_t timeout = 1000){
  uint64_t start = millis();
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    if(millis() - start > 1000){
      logger->log("Wifi connection timeout", ERROR);
      return;
    }
  }
  logger->log(WiFi.localIP(), INFO);
}

void InfluxServer::connectToInfluxDB(uint64_t timeout = 10000){
  uint64_t start = millis();
  while(!client.validateConnection()){
    delay(500);
    Serial.print(".");
    if(millis() - start > timeout){
      logger->log("InfluxDB connection timeout", ERROR);
      logger->log(client.getLastErrorMessage(), ERROR);
      return;
    }
  }
}

void InfluxServer::uploadPoint(Point point){
  if(!WiFi.isConnected()){
    logger->log("Wifi connection lost", ERROR);
    connectToWifi();
    return;
  }
  if(!client.isConnected()){
    logger->log("InfluxDB connection lost", ERROR);
    connectToInfluxDB();
    return;
  }

  if (!client.writePoint(point)) {
    logger->log("InfluxDB write failed: ", ERROR);
    logger->log(client.getLastErrorMessage(), ERROR);
  }
}