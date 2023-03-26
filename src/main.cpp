#include "util.h"
#include "battery.h"
#include "credentials.h"
#include "logger.h"
#include <InfluxDbClient.h>


Battery battery;
Logger logger(&Serial, DEBUG);
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);

void connectToWifi(uint64_t timeout = 10000){
  uint64_t start = millis();
  WiFi.begin(SSID, PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
    if(millis() - start > timeout){
      logger.log("Wifi connection timeout", ERROR);
      return;
    }
  }
  logger.log(WiFi.localIP(),INFO);
}

void connectToInfluxDB(uint64_t timeout = 10000){
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

void setup() {
  Serial.begin(115200);
  battery = Battery(BATTERY_PIN);

  logger.log("Connecting to Wifi",INFO);
  connectToWifi();

  logger.log("Setting time",INFO);
	timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  logger.log("Connecting to InfluxDB",INFO);
  connectToInfluxDB();

  logger.log("Setup completed",INFO);
}

void loop(){
  Point point("Serra");
  point.addField("battery", battery.getCharge());

  if(!WiFi.isConnected()){
    logger.log("Wifi connection lost", ERROR);
    connectToWifi();
    return;
  }
  if(!client.isConnected()){
    logger.log("InfluxDB connection lost", ERROR);
    connectToInfluxDB();
    return;
  }

  if (!client.writePoint(point)) {
    logger.log("InfluxDB write failed: ", ERROR);
    logger.log(client.getLastErrorMessage(), ERROR);

  }
  delay(1000);
}