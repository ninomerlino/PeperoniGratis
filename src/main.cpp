#include "util.h"
#include "battery.h"
#include "logger.h"
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <WiFiMulti.h>

Battery battery;
Logger logger;
InfluxDBClient server;
WiFiMulti wifiMulti;
Point point("esp32");

void setup() {
  randomSeed(110);
  Serial.begin(115200);
  logger = Logger(&Serial, LogLevel::DEBUG);
  battery = Battery(BATTERY_PIN);
  server = InfluxDBClient(INFLUXDB_URL_MACRO, INFLUXDB_ORG_MACRO, INFLUXDB_BUCKET_MACRO, INFLUXDB_TOKEN_MACRO, InfluxDbCloud2CACert);

  logger.log("Connecting to Wifi", LogLevel::INFO);
  wifiMulti.addAP(SSID_MACRO, PASSWORD_MACRO);
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(1000);
    logger.log(".", INFO);
  }
  logger.log(WiFi.localIP(), INFO);

  logger.log("Setting time", LogLevel::INFO);
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  logger.log("Connecting to InfluxDB", LogLevel::INFO);
  while (!server.validateConnection()) {
    delay(1000);
    logger.log(".", INFO);
  }
  logger.log("Setup completed", LogLevel::INFO);
}

void loop() {
  point.clearFields();
  point.addField("battery", random(100));
  point.addField("moisture", random(1000) / 100.0);
  point.addField("temperature", random(30));
  point.addField("humidity", random(100));
  if (wifiMulti.run() != WL_CONNECTED) {
    logger.log("Wifi connection lost", ERROR);
    while (wifiMulti.run() != WL_CONNECTED) {
      delay(1000);
      logger.log(".");
    }
    logger.log(WiFi.localIP());
  }
  if(!server.validateConnection()){
    logger.log("InfluxDB connection lost", ERROR);
      logger.log(server.getLastErrorMessage(), ERROR);
  }
  logger.log("Writing point", DEBUG);
  if (!server.writePoint(point)) {
    logger.log("InfluxDB write failed: ", ERROR);
    logger.log(server.getLastErrorMessage(), ERROR);
  }
  delay(1000);
}