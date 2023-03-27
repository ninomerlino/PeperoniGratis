#include "util.h"
#include "battery.h"
#include "logger.h"
#include "serverConnection.h"


Battery battery;
Logger logger;
InfluxServer server;

void setup() {
  randomSeed(110);
  Serial.begin(115200);
  logger = Logger(&Serial, LogLevel::DEBUG);
  battery = Battery(BATTERY_PIN);

  logger.log("Connecting to Wifi", LogLevel::INFO);
  server.connectToWifi();
  logger.log("Setting time", LogLevel::INFO);

	timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  logger.log("Connecting to InfluxDB", LogLevel::INFO);
  server.connectToInfluxDB();
  logger.log("Setup completed", LogLevel::INFO);
}

void loop(){
  Point point("esp32");
  point.addTag("STATUS","RETARDED");
  point.addField("battery", random(100));
  point.addField("moisture",random(1000)/100.0);
  point.addField("temperature", random(30));
  point.addField("humidity", random(100));
  logger.log("Writing point",DEBUG);
  server.uploadPoint(point);
  delay(1000);
}