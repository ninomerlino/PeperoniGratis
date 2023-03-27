#include "util.h"
#include "battery.h"
#include "logger.h"
#include "serverConnection.h"


Battery battery;
Logger logger;
InfluxServer server;

void setup() {
  Serial.begin(115200);
  logger = Logger(&Serial, LogLevel::DEBUG);
  battery = Battery(BATTERY_PIN);
  server = InfluxServer(&logger);

  logger.log("Connecting to Wifi", LogLevel::INFO);

  logger.log("Setting time", LogLevel::INFO);

	timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  logger.log("Connecting to InfluxDB", LogLevel::INFO);

  logger.log("Setup completed", LogLevel::INFO);
}

void loop(){
  Point point("Serra");
  point.addField("battery", battery.getCharge());
  point.addField("moisture", 16);
  point.addField("temperature", 24.5);
  point.addField("humidity", 75);
  server.uploadPoint(point);
  delay(1000);
}